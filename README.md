This project is to create a simple compiler with features such as: variable declaration, print, basic arithmetic operations, if condition, simple for loop, simple while loop, simple function, comment, error handling. 
below are some examples related to these features in order to try it.

look at these example to have an idea about the language syntax

variable declaration and print:
x = 10
print x
expected output : 10

basic arithmetic operation: 
x = 5 + 3
print x
expected output : 8

x = 5 * 3
print x
expected output : 15

x = 5 - 3
print x
expected output : 2

x = 15 / 3
print x
expected output : 5

if condition : 
x = 10
if x > 5 then print "x is greater" else print "x is smaller"
expected output : x is greater

for loop :
for i = 1 to 5 do print i
expected output :
1
2
3
4
5



while loop :
x = 1
while x <= 5 do
    print x
    x = x + 1
end
expected output : 
1
2
3
4
5


function : 
func addNumbers a b
    result = a + b
    print result
end

call addNumbers 5 10
expected output :
15

comment:
#this is a compiler project

error handling:
x = "hello" + 5
expected output : Error: Invalid token '"hello"'



