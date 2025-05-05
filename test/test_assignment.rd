var myint = 10
var myref = myint

fn show() {
   print("---------------")
   print("myint:", myint)
   print("myref:", myref)
   print("---------------")
}

show()

print("SETTING myint = 20")
myint = 20

show()

print("SETTING myref = 30")
myref = 30

show()
