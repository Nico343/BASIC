#include "Calculator.cpp"
#include <iostream>

using namespace std;

// te_parser tep;

int main()
{
    // printf("This project uses the C++17 language standard version.\n");
    // test();
    // prin();

    // RS485_read();

    // push('1');
    // push('2');
    // push('3');
    // push('4');
    // printf(buf);
    // char lastelement = back();
    // printf("\nLast element: %c\n", lastelement);
    // lastelement = pop();
    // lastelement = pop();
    // printf(buf);
    // printf("\n2nd Popped element: %c\n", lastelement);
    // push('5');
    // push('6');
    // lastelement = pop();
    // lastelement = pop();
    // lastelement = pop();
    // lastelement = pop();
    // printf("Is buffer empty?\n%s\n", isEmpty() ? "Yes" : "No");
    // lastelement = back();
    // printf("last element: %c\n", lastelement);
    // push('7');
    // push('8');
    // printf(buf);

    int val = 1200;
    char Expression[30];
    // snprintf(Expression, sizeof(Expression), "(%d/10)-40", val);
    snprintf(Expression, sizeof(Expression), "(%d-5)*10-22/2", val); //=11939
    double r = EvaluateExpression(Expression);
    // printf("The expression:%s %s %s\n\t", string(Expression), "\nevaluates to:\n\t", to_string(r));
    cout << "\nExpression 1:\n\t" << Expression << "\nevaluates to:\n\t" << r << "\n";
    
    snprintf(Expression, sizeof(Expression), "(%d/10)-4*23", val); //=28
    r = EvaluateExpression(Expression);
    // printf("The expression:%s %s %s\n\t", string(Expression), "\nevaluates to:\n\t", to_string(r));
    cout << "\nExpression 2:\n\t" << Expression << "\nevaluates to:\n\t" << r << "\n";

    snprintf(Expression, sizeof(Expression), "(%d+10)-46/230", val); //=1209.8
    r = EvaluateExpression(Expression);
    // printf("The expression:%s %s %s\n\t", string(Expression), "\nevaluates to:\n\t", to_string(r));
    cout << "\nExpression 3:\n\t" << Expression << "\nevaluates to:\n\t" << r << "\n";
    
    snprintf(Expression, sizeof(Expression), "(%d+10)/1000-46/23", val); //=-0.79
    r = EvaluateExpression(Expression);
    // printf("The expression:%s %s %s\n\t", string(Expression), "\nevaluates to:\n\t", to_string(r));
    cout << "\nExpression 4:\n\t" << Expression << "\nevaluates to:\n\t" << r << "\n";
    
    /**
     * an operation that can handle concatenation of two registers into a single value
     * (operation name: concat?)
     */
    return 0;
} 