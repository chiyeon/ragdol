var BTN_UP = 0
var BTN_DOWN = 1
var BTN_LEFT = 2
var BTN_RIGHT = 3

var x = 20
var y = 20

fn update() {
   draw_text(4, 4, "HELLO WORLD")
   draw_text(x, y, "P")

   if (is_button_held(BTN_UP)) {
      y = y - 1
   } 
   if (is_button_held(BTN_DOWN)) {
      y = y + 1
   }
   if (is_button_held(BTN_LEFT)) {
      x = x - 1
   }
   if (is_button_held(BTN_RIGHT)) {
      x = x + 1
   }
}

init_window(update)

