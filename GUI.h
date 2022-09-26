typedef struct {
	char* title;
	struct node* parent;
	struct node* child [10];
	void (*fun_ptr)(void);
	uint8_t index;
	} node;

node* new_node(char* name, node* parent) {
	node* node = malloc(sizeof(node));
	node->title = name;
	node->parent = parent;
	for(int i=0; i<10; i++){
		(node->child)[i] = NULL;
	}
	node->index = NULL;
	return node;
}

#define N_NODES 4
#define END_MAIN_MENU 3

char* curr_mode="";
uint8_t curr_arrow_pos=0;



void set_current_node(node*n){
	curr_mode=n->title;
}

char* get_current_node(void){
	return curr_mode;
}

void GUI_main_menu(uint8_t page){
	OLED_clear_menu();
	OLED_goto_pos(1,8);
	OLED_print_string("PING PONG GAME");
	if(page ==0){
		OLED_goto_pos(3,32);
		OLED_print_string("Play");
		OLED_goto_pos(5,32);
		OLED_print_string("Quit");
	}
	else if(page == 1){
		OLED_goto_pos(3,32);
		OLED_print_string("Settings");
		OLED_goto_pos(5,32);
		OLED_print_string("Properties");
	}
}

void set_child(node* parent, node* child, uint8_t i){
	parent->child[i] = child;
	child->index = i;
}

void set_fun(node* n, void (* fun_ptr)(void)){
	n->fun_ptr= fun_ptr;
}


void f_play(){
	OLED_clear();
	OLED_goto_pos(3,32);
	OLED_print_string("Play");
}

void f_sr_highscore(){
	OLED_clear();
	OLED_goto_pos(3,32);
	OLED_print_string("highscore");
}

void f_calibrate(){
	OLED_clear();
	OLED_goto_pos(3,32);
	OLED_print_string("calibrate");
}

void f_set_diff(){
	OLED_clear();
	OLED_goto_pos(3,32);
	OLED_print_string("set diff");
}


void GUI_menu_init(){

	node* main_menu= new_node("Main Menu", NULL);
	curr_mode= main_menu->title;
	node* play= new_node("Play", main_menu);
	node* sr_highscore= new_node("See/Reset Highscore", main_menu);
	node* calibrate= new_node("Calibrate", main_menu);
	node* set_diff= new_node("Set Difficulty", main_menu);
	
	set_child(main_menu, play, 0);
	set_child(main_menu, sr_highscore, 1);
	set_child(main_menu, calibrate, 2);
	set_child(main_menu, set_diff, 3);	
	
	set_fun(main_menu,GUI_main_menu);
	set_fun(play,f_play);
	set_fun(sr_highscore,f_sr_highscore);
	set_fun(calibrate,f_calibrate);
	set_fun(set_diff,f_set_diff);
	GUI_main_menu(0);
	OLED_print_arrow(3,15);
		  printf("CURRENT ARROW=%d\n",curr_arrow_pos);
	
}

void move_arrow(uint8_t dir){
	if (dir==0 ){
	  OLED_clear_arrow();  
	  OLED_print_arrow(5,15);
	  } 
	else if (dir == 1){
	OLED_clear_arrow();
	OLED_print_arrow(3,15);
	}
	
	
}

void menu_navigation(){
	
	joystick = get_joypos();
	
	/*if (joystick.y_currdir == "UP"){
		f_play();
		printf("fhdhisfdhsdfhsfdhsd \n");*/
	
	if (curr_mode=="Main Menu"){
			if(joystick.y_currdir == "DOWN" && curr_arrow_pos<END_MAIN_MENU){
				
					
				if (curr_arrow_pos%2 ==0 ){		
					move_arrow(0);
					curr_arrow_pos=curr_arrow_pos+1;

				}
				else GUI_main_menu(((int)(curr_arrow_pos/2))+1);
						
			}
			else if(joystick.y_currdir == "UP" && curr_arrow_pos>0){
				
				if (curr_arrow_pos%2 ==1 ){		
					move_arrow(1);	
					curr_arrow_pos=curr_arrow_pos-1;
						
			}
			else GUI_main_menu(((int)(curr_arrow_pos/2))-1);
		
	
	
	}
	printf("CURRENT ARROW=%d\n",curr_arrow_pos);
	}
	}
