var a = 20

fn print_custom(a) {
   print("A is:", a)
}

print("There is a global var a:", a)

print("Now lets test using a local var.")
print_custom("This is the function arg a (1)")

print("Lets take it a step inwards")
{
   var a = 30
   print("Were inside a scope step. A is now", a)
   print("Lets call function again")
   print_custom("This is the function arg a (2)")
   print("Leaving this scope")
}

print("Back to global scope, and A is", a)
