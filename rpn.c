//
//  main.c
//  rpn
//
//  Created by William McCarthy on 192//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//Homework by AMY NGUYEN 2/24/20

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100
#define MAXVAR 26
#define MATH 'n'
#define VARIABLE 'v'


size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

void clear(void)
{
	sp = 0;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';

  if(c >= 'a' && c <= 'z'){
      i = 0;
      while (isalpha(s[++i] = c = getch_()));
      s[i] = '\0';
      return MATH;
    }
  if (c == '=' || c == '?'){
     i = 0;
     while (isalpha(s[++i] = c = getch_()));
     s[i] = '\0';
     return VARIABLE;
   }
  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return
  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if(c == '-'){
    if(isdigit(c=getch_()) || c == '.'){
      s[++i]=c;
    }
    else{
      if(c!=EOF)
      ungetch_(c);
      return '-';
    }
    }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}

void  math(char* s){
  double op1, op2, result = 0;

  if (strcmp(s, "pow") == 0) {
    op2 = pop();
    op1 = pop();
    result = pow(op1, op2);
  }
  else if (strcmp(s, "sin") == 0){ result = sin(pop()); }
  else if (strcmp(s, "cos") == 0){ result = cos(pop()); }
  else if (strcmp(s, "exp") == 0){ result = exp(pop()); }
  push(result);
  printf("%.2f\n", result);
}
void variable(int v, double var[]){
  pop();
  if(v >= 'A' && v <= 'Z') {
  var[v -'A'] = pop();
  }
}

void rpn(void) {
  int type, v;
  double op2, op1, value;
  double var[MAXVAR];
  char s[BUFSIZ];

  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':
      value = pop();
      printf("\t%.8g\n",value);
      break;
      case NUMBER:  push(atof(s));              break;
      case MATH:    math(s);
      case VARIABLE: variable(v, var);
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
      if ((op2 = pop()) == 0.0) { fprintf(stderr, "mod by zero\n");  break; }
      push((int)pop() % (int)op2);
      break;
      case 'p': //print
      op2 = pop();
        printf("\t%.8g\n", op2);
        push(op2);
        break;
      case 'c':
      clear(); break;
      case 'd': //duplicate function
      op2 = pop();
      push(op2);
      push(op2);
      break;
      case 's': //swap function
      op2 = pop();
      op1 = pop();
       push(op2);
       push(op1);
       break;
      case '='://variable function
      pop();
    if(v >= 'A' && v <= 'Z'){
      var[v - 'A'] = pop(); break;
    }
      default:
      if(type >= 'A' && type <= 'Z') { push(var[type - 'A']); break; }
      if(type =='v') { push(value); break; }
       fprintf(stderr, "unknown command %s\n", s);  break;
    }
    v = type;
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
