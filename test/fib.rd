fn fib(n) {
   if (n <= 1) {
      return n;
   }

   return fib(n - 1) + fib(n - 2);
}

var i = 0;
while (i < 10) {
   i = i + 1;
   print(fib(i));
}
