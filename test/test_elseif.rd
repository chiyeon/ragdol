fn rate_grade(grade) {
   if (grade == 100) {
      print("You got a perfect score!")
   } elif (grade >= 90) {
      print("You got an A at least")
   } elif (grade >= 80) {
      print("You got a B not bad")
   } elif (grade >= 70) {
      print("Cs get degrees")
   } elif (grade >= 60) {
      print("Might wanna hit the books")
   } else {
      print("Nah youre cooked bud")
   }
}

rate_grade(100)
rate_grade(93.0)
rate_grade(83.2)
rate_grade(70)
rate_grade(60.3)
rate_grade(23)
