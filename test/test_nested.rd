fn outer() {
   return inner();
}

fn inner() {
   return "Message from my inner function"
}

var foo = "You should see this"
fn outer2() {
   var foo = "HAHA HIJACKED"
   return inner2()
}

fn inner2() {
   return foo;
}

print(outer());
print(outer2())
