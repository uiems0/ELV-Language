Math is easy in this language.

OPERATORS:
+ (Addition, adds two numbers)
- (Subtraction, subtracts first number by second)
* (Multiplication, multiplies two numbers)
/ (Division, Divides first number by second, 1/2 = 0.5)
^ (Powers, Raises the first number to the second power(Use this for square roots with things like 5^0.5 for the square root of 5))
% (Modulus,  runs the Modulus function on the first number, with the second being its divisor (Runs the same as C processes it))

SYNTAX:
VARNAME = (Math)

EXAMPLES:
Var Num = 5
Num = Num + 2 

~ Num now equals 7

Var Num2 = 9
Num2 = Num2 ^ 0.5

~ Num2 now equals 3

Var Num3 = 10
Var Num3 = Num3%9

~ Num3 now equals 1

RULES AND OTHER THINGS YOU CAN DO:

The operators follow the orders of operations, parenthesis will work

If you do (2+1)4 the program will interpret it as (2+1)*4

if you do 4(2+1) the program will interpret it as 4*(2+1)

Doing things like Num += 1 will error, assignment compound operators do not work

The math follows the orders of operations

Spaces in the math are stripped from the equation

Putting a non integer, negative, or 0 anywhere in the divisor or devised in modulus return 0 no matter what


~ This is implemented, but needs testing and the modulus cant detect if a number is a int or not
