var mymsg = "this is my message, aka mymsg"
var myint = 10

fn myprint(str) {
   print(str);
}

fn myprint2(a, b) {
   print("MYPRINT2 START")
   print("Woah, I was given A:", a, "and B:", b)
   print("MYPRINT2 END")
}

print("myint:", myint)
print("mymsg:", mymsg)

print("This is a message using my custom myprint method that takes a variable:")
myprint("hello")
myprint(mymsg)
myprint(myint)

print("Now i wil test 2 arguments with the myprint2 function")
myprint2("AYE", "BEE")
myprint2(myint, "not an int")
