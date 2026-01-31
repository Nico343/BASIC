/**
 * Forked from https://github.com/creme332/basic-console-calculator.git
 * 
 * Intended for Arduino
 * 
 * For C_Execuatble:
 * Replace all Serial.println with printf
 * replace all String with string
 * using namespace std;
 */

// #include <Arduino.h> //Might be needed for platformio

#define BUFFER_SIZE 64
char buf[BUFFER_SIZE];
volatile int writePointer = 0;

bool push(char data)
{
    int nextWritePointer = (writePointer + 1) % BUFFER_SIZE;
    if (nextWritePointer == BUFFER_SIZE)
    {
        Serial.println("Parse operator stack overflow\nAdd more space to the buffer.\n");
        return false;
    }
    buf[writePointer] = data;
    writePointer = nextWritePointer;
    return true;
}
char pop()
{
    if (writePointer == 0)
    {
        // Buffer is empty, return a null char or handle as error
        return '\0';
    }
    char data = buf[writePointer - 1];
    writePointer = (writePointer - 1) % BUFFER_SIZE;
    return data;
}

char back()
{
    if (writePointer == 0)
    {
        // Buffer is empty, return a null char or handle as error
        return '\0';
    }
    int lastIndex = (writePointer - 1) % BUFFER_SIZE;
    return buf[lastIndex];
}

int length()
{
    return writePointer;
}

bool isEmpty()
{
    return writePointer == 0;
}

void empty()
{
    // clear the operator stack
    writePointer = 0;
}

double dbuf[BUFFER_SIZE];
volatile int dWP = 0;

bool dpush(double data)
{
    int nextdWP = (dWP + 1) % BUFFER_SIZE;
    if (nextdWP == BUFFER_SIZE)
    {
        // Buffer is full, data lost (handle as needed)
        Serial.println("Parse operator double stack overflow\nAdd more space to the buffer.\n");
        return false;
    }
    dbuf[dWP] = data;
    dWP = nextdWP;
    return true;
}

double dpop()
{
    if (dWP == 0)
    {
        // Buffer is empty, return a null char or handle as error
        return '\0';
    }
    double data = dbuf[dWP - 1];
    dWP = (dWP - 1) % BUFFER_SIZE;
    return data;
}

double dback()
{
    if (dWP == 0)
    {
        // Buffer is empty, return a null char or handle as error
        return '\0';
    }
    int lastIndex = (dWP - 1) % BUFFER_SIZE;
    return dbuf[lastIndex];
}

// returns precedence of operators
int precedence(char op)
{
    switch (op)
    {
    case '-':
        return 2;
    case '+':
        return 2;
    case '/':
        return 3;
    case '*':
        return 3;
    case '^':
        return 4;
    case 'm':
        return 5; // unary minus
    case 'p':
        return 5; // unary plus
    }
    return -1;
}

// L : left associativity R: right associativity
char associativity(char op)
{
    // Could be code golfed?
    switch (op)
    {
    case '-':
        return 'L';
    case '+':
        return 'L';
    case '/':
        return 'L';
    case '*':
        return 'L';
    case '^':
        return 'R';
    case 'm':
        return 'R'; // unary minus
    case 'p':
        return 'R'; // unary plus
    }
    return '.';
}

bool isoperator(char op)
{
    // used to identify unary operators. A unary operator appears just after the following op.
    // Could be code golfed?
    switch (op)
    {
    case '-':
        return 1;
    case '+':
        return 1;
    case '/':
        return 1;
    case '*':
        return 1;
    case '^':
        return 1;
    case '(':
        return 1;
    }
    // Note a minus/plus following ) is not a unary minus/plus
    return 0;
}

String modify(String s)
{
    // Remove all spaces from s
    // Replace unary plus with p
    // Replace unary minus with m
    // add * before ( if there's no operator
    for (char &c : s) // Could be code golfed?
    {
        if (c == ' ')
        {
            c = '\0';
        }
    }
    // Replace unary plus with p
    // Replace unary minus with m
    for (long long i = 1; i < s.length(); i++)
    {
        if (isoperator(s[i - 1]) && (s[i] == '-' || s[i] == 'm'))
        {
            s[i] = 'm';
        }
        if (isoperator(s[i - 1]) && (s[i] == '+' || s[i] == 'p'))
        {
            s[i] = 'p';
        }
    }
    if (s[0] == '-')
    {
        s[0] = 'm';
    }
    if (s[0] == '+')
    {
        s[0] = 'p';
    }

    // add * before ( if there's no operator
    String ans = "";
    for (int i = 0; i < s.length() - 1; i++)
    {
        if (s[i] >= '0' && s[i] <= '9' && s[i + 1] == '(')
        {
            ans += s[i];
            ans += '*';
        }
        else
        {
            ans += s[i];
        }
    }
    ans += s[s.length() - 1];
    return ans;
}

String to_postfix(String exp)
{
    // Implementation of shunting yard algorithm.
    exp = modify(exp);
    String ans = "";
    String operand = "";
    char prevchar = '*';

    for (long long i = 0; i < exp.length(); i++)
    {

        if (i > 0)
        {
            prevchar = exp[i - 1];
        }
        char c = exp[i];

        if (isdigit(c) != 0 || c == '.')
        { // if c is a digit or a decimal point
            operand += c;
        }
        else
        { // c is an operator
            if (operand != "")
            {
                ans += operand + " ";
                operand = "";
            }
            if (length() == 0 || c == '(')
            {
                push(c);
            }
            else
            {                      // operator stack not empty
                char top = back(); // operator at top of operator stack
                if (c == ')')
                {
                    while (top != '(')
                    {
                        ans += top;
                        ans += " ";
                        pop();
                        top = back();
                    }
                    pop();
                }
                else
                {
                    if (top == '(' || (associativity(c) == 'R' && (precedence(c) >= precedence(top))))
                    {
                        push(c);
                    }
                    else
                    {
                        if (associativity(c) == 'L')
                        {
                            while (precedence(c) <= precedence(top) && length() != 0 && top != '(')
                            {
                                ans += top;
                                ans += " ";
                                pop();
                                if (length() == 0)
                                {
                                    break;
                                }
                                top = back();
                            }
                            push(c);
                        }
                    }
                }
            }
        }
    }

    if (operand != "")
    {
        ans += operand + " ";
    }
    if (length() != 0)
    {
        for (int i = length() - 1; i > -1; i--)
        {
            ans += buf[i];
            ans += " ";
        }
    }
    return ans;
}

char tokens[50][20];
int token_count = 0;
int char_id = 0;

void tokenizer(String s)
{
    for (int i = 0; i <= s.length(); i++)
    {
        // If we hit a space or end of String, we've finished one token
        if (s[i] == ' ' || s[i] == '\0')
        {
            if (char_id > 0)
            {
                tokens[token_count][char_id] = '\0'; // Null-terminate the String
                token_count++;
                char_id = 0;
            }
        }
        else
        {
            tokens[token_count][char_id++] = s[i];
        }
    }
}

double pow(double base, double exp)
{
    double result = 1.0;
    for (int i = 0; i < (int)exp; i++)
    {
        result *= base;
    }
    return result;
}

double result(double a, double b, String o)
{
    // result(5.2, 1.1, "-") returns 4.1
    if (o == "+")
    {
        return a + b;
    }
    if (o == "-")
    {
        return a - b;
    }
    if (o == "*")
    {
        return a * b;
    }
    if (o == "/")
    {
        return a / b;
    }
    if (o == "^")
    {
        return pow(a, b);
    }
    return 0;
}

/**
 * Valid Expressions
 * 112+22*(2*(22-53^2*(2))-7/(7/2-1/4)) //-246159.38462
 * 2+-1 // 1
 * 2^0.5 // 1.413
 * 7^7^(2/(4-532)^3) // 7^(7^(2/(4-532)^3)) = 7.0
 * 2(2)  // same as 2*2

 * Inavlid Expressions
 * 1---1 //not supported yet
 * 2* //incomplete expression
 * 7 ^ 7 ^ (2/(4-532)^3) //    (No spaces between digits/operators
                                are allowed when inputting directly
                                from console)
 */
double EvaluateExpression(String exp)
{
    empty();
    String postfix = to_postfix(exp); // postfix has spaces but no ()
    tokenizer(postfix);
    double a, b;

    for (int i = 0; i < token_count; i++)
    {
        String toki = String(tokens[i]);
        if (toki == "m")
        { // unary minus
            a = dback();
            dpop();
            dpush(a * -1);
        }
        else
        {
            if (toki == "*" || toki == "/" || toki == "+" || toki == "-" || toki == "^")
            {
                b = dback();               // Move last operand to b
                dpop();                    // remove last operand
                a = dback();               // Move new last operand to a
                dpop();                    // remove new last operand
                dpush(result(a, b, toki)); // evaluate and push the result to the back
            }
            else
            {
                dpush(stod(toki)); // convert String to double and push to back
            }
        }
    }

    b = dback();
    dWP = 0;
    token_count = 0;
    return b;
}