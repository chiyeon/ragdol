var myvar = 10
var mystr = "Hello world!!!"
var myfloat = 3.1415926

fn print_story() {
   print("This is a story that is getting printed from the print function");
}

fn inc() {
   myvar = myvar + 1;
}

print("This is a test of the print function, multiple inline printing.")
print("mystr:", mystr, "; myvar:", myvar, "; myfloat:", myfloat)

print('more printing from functions, and now a functioncall:')

print()
print_story()
print()

print("10 * 100 + (3 - 200):", 10 * 100 + (3 - 200))

print("heres a test of the incrementing function")
print("myvar:", myvar, "(now calling)");
inc()
print("myvar:", myvar, "(now calling)");
inc()
print("myvar:", myvar, "(now calling)");
inc()
print("myvar:", myvar, "(now calling)");
inc()
