var sbtrue1 = true || false
var sbtrue2 = true && true
var sbtrue3 = false || true || false || false || false
var sbtrue4 = true && false || false || false && false || true && false || true && true
var sbtrue5 = false || (true && false || true && false && false || false || true) || false

var sbfalse1 = false && true
var sbfalse2 = false && false && false || false || false || false && false || false && true || false && true
var sbfalse3 = false || false

print("should be true:")
print(sbtrue1, sbtrue2, sbtrue3, sbtrue4, sbtrue5)
print()
print("should be false:")
print(sbfalse1, sbfalse2, sbfalse3)
