# Lab3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 25

struct stack
{
	char symbol;
	struct stack *next;
};

char* memory_allocation(char *string);
char* input_string(char *string);
char* string_revers(char *string);
char* signs_in_prior(char *string);
int check_infics_string(char *string);
int check_russian_letters(char *string);
void input_stack(char *string, char *outstring);
void output_stack(struct stack* current, int element, char *outstring);
void func_output(char *outstring);
int func_repeat(int element);
int check_flag(void);
int check_stack(struct stack **top);
void clean_stdin(void);
int prior(char);

struct stack* push(struct stack *top, char symbol);
char pop(struct stack **top);

int main()
{
	int flag = 0, n;
	char *string = { NULL }, *outstring = { NULL };
	do
	{
		string = memory_allocation(string);
		if (!string)
		{
			printf("Memory is not allocated\n");
			return 0;
		}
		outstring = memory_allocation(outstring);
		if (!outstring)
		{
			printf("Memory is not allocated\n");
			free(string);
			return 0;
		}
		printf("\t\tInput expression without numbers, spaces and brackets please\n\nFor example a+b*c-d^a:\t");
		do
		{
			string = input_string(string);
			flag = check_infics_string(string);
		} while (flag == 1);
		n = strlen(string);
		input_stack(string, outstring);
		flag = func_repeat(flag);
	} while (flag == 1);
	return 0;
}

char* memory_allocation(char *string)
{
	string = (char*)calloc((N + 1), sizeof(char));
	if (!string)
	{
		printf("Memory is not allocated\n");
		return 0;
	}
	return string;
}
char* input_string(char *string)
{
	int i = 0, n = 0, flag1;
	do
	{
		flag1 = 0;
		fgets(string, N + 1, stdin);
		n = strlen(string);
		if (n == 1)
		{
			printf("\nEmpty input, try again: ");
			flag1 = 1;
			continue;
		}
		for (i = 0; i < n; i++)
		{

			if ((string[i] >= '0' && string[i] <= '9') || string[i] == ' ' || string[i] == '(' || string[i] == ')')
			{
				printf("\nYou use numbers, spaces or brackets in expression, input it again: ");
				flag1 = 1;
				break;
			}
		}
	} while (flag1 == 1);
	return string;
}
char* string_revers(char *string)
{
	char c = 0;
	int i, j, n = strlen(string);
	for (i = 0, j = (n - 1); i < (n - 1) / 2 && j > (n - 1) / 2; i++, j--)
	{
		c = string[i];
		string[i] = string[j];
		string[j] = c;
	}
	return string;
}
int check_infics_string(char *string)
{
	int i, n = strlen(string), flag = check_russian_letters(string);
	if (flag == 1) return 1;
	if (prior(string[0]) > 0 || prior(string[n - 2]) > 0)
	{
		printf("\nString was inputed not in infics form, input it again: ");
		return 1;
	}
	for (i = 0; i < n; i++)
	{
		if (string[i] == '\n' || string[i] == '\0')
		{
			return 0;
		}
		if (string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/' || string[i] == '^')
		{
			if (string[i + 1] == '+' || string[i + 1] == '-' || string[i + 1] == '*' || string[i + 1] == '/' || string[i + 1] == '^')
			{
				printf("\nString was inputed not in infics form, input it again: ");
				return 1;
			}
		}
	}
	return 0;
}
int check_russian_letters(char *string)
{
	int i = 0, j, n = strlen(string);
	const char *massive = { "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁё#:;&!.,~`'" };
	while (massive[i++] != '\0')
	{
		for (j = 0; j < (n - 1); j++)
		{
			if (massive[i] == string[j])
			{
				printf("\nYou use forbidden symbols in string, input in again:\t");
				return 1;
			}
		}
	}
	return 0;
}
void input_stack(char *string, char *outstring)
{
	int i = 0, n = strlen(string);
	struct stack *parse = { NULL };
	for (i = 0; i < n; i++)
	{
		if (string[i] == '\n' || string[i] == '\0') break;
		parse = push(parse, string[i]);
	}
	free(string);
	output_stack(parse, n, outstring);
}
void output_stack(struct stack* current, int element, char *outstring)
{
	int i;
	for (i = 0; i < (element - 1); i++)
	{
		outstring[i] = pop(&current);
	}
	string_revers(outstring);
	func_output(outstring);
	free(current);
}
void func_output(char *outstring)
{
	int i = 0, n = strlen(outstring);
	char c;
	for (i = 0; i < n; i++)
	{
		if (prior(outstring[i]) > 0)
		{
			c = outstring[i - 1];
			outstring[i - 1] = outstring[i];
			outstring[i] = c;
			i++;
		}
	}
	for (i = 2; i < n; i++)
	{
		if (prior(outstring[i - 2]) > prior(outstring[i]) && (prior(outstring[i - 2]) > 0) && (prior(outstring[i]) > 0))
		{
			c = outstring[i - 2];
			outstring[i - 2] = outstring[i];
			outstring[i] = outstring[i - 1];
			outstring[i - 1] = c;
		}
	}
	printf("\nThis expression in prefics form:\t%s\n", outstring);
	free(outstring);
}
struct stack* push(struct stack *top, char symbol)
{
	struct stack *str;
	str = (struct stack*)calloc(1, sizeof(struct stack));
	if (!str)
	{
		printf("Memory is not allocated\n");
		exit(-1);
	}
	str->symbol = symbol;
	str->next = top;
	return str;
}
char pop(struct stack **top)
{
	char c = 0;
	struct stack *str = NULL;
	if (top == NULL)
	{
		return 0;
	}
	str = *top;
	c = str->symbol;
	*top = str->next;
	free(str);
	return c;
}
int check_flag(void)
{
	int element = 0, element1 = 0;
	do
	{
		element1 = scanf("%d", &element);
		clean_stdin();
		if (element1 != 1)
		{
			printf("\nInvalid input, try again: ");
		}
		else if (element < 1 || element > 2)
		{
			printf("\nInvalid input, try again: ");
		}
		else break;
	} while (1);
	return element;
}
int check_stack(struct stack **top)
{
	if (*top == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int func_repeat(int element)
{
	int flag = 0;
	printf("\nDo you want to input another expression? - press 1 if you want or 2 if don't: ");
	flag = check_flag();
	if (flag == 1)
	{
		element = 1;
		return element;
	}
	else if (flag == 2)
	{
		printf("Thanks for work with us\n");
		element = 0;
		return element;
	}
	else
	{
		printf("Invalid input. Program will stop\n");
		return element;
	}
}
int prior(char symbol)
{
	switch (symbol)
	{
	case '^':			return 3;
	case '*': case '/': return 2;
	case '-': case '+': return 1;
	default:			return 0;
	}
}
void clean_stdin(void)
{
	int c;
	do
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}
