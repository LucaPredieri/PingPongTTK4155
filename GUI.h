typedef struct {
	char* title;
	struct node* parent;
	struct node* child [5];
	void (*fun_ptr)(void);
	uint8_t index;
	} node;

node* new_node(char* name, node* parent) {
	node* n = malloc(sizeof(node));
	n->title = name;
	n->parent = parent;
	for(int i=0; i<5; i++){
		(n->child)[i] = NULL;
	}
	n->index = NULL;
	return n;
}

#define N_NODES 4
#define END_MAIN_MENU 4

node* curr_mode;
uint8_t curr_arrow_pos=0;
uint8_t value;
uint8_t flag = 0;

volatile node* main_menu;
volatile node* play;
volatile node* sr_highscore;
volatile node* calibrate;
volatile node* set_diff;
volatile node* quit;


void set_current_node(node*n){
	curr_mode=n;
}

node* get_current_node(void){
	return curr_mode;
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

void f_quit(){
	OLED_clear();
	OLED_goto_pos(3,32);
	OLED_print_string("bye bye");
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


void GUI_init(){
	OLED_clear();
	OLED_goto_pos(1,8);
	OLED_print_string("PING PONG GAME");
	OLED_goto_pos(3,32);
	OLED_print_string("Play");
	OLED_goto_pos(5,32);
	OLED_print_string("See/reset hs");		
	
}

void GUI_main_menu(uint8_t page){
	set_current_node(main_menu);
	OLED_clear();
	OLED_goto_pos(1,8);
	OLED_print_string("PING PONG GAME");

	if(page ==2){
		OLED_goto_pos(3,32);
		OLED_print_string("Quit");
		move_arrow(curr_arrow_pos%2);
		
	}
	else if(page == 1){
		OLED_goto_pos(3,32);
		OLED_print_string("Calibrate");
		OLED_goto_pos(5,32);
		OLED_print_string("Set difficulty");
		move_arrow(curr_arrow_pos%2);
	}
	else {
		OLED_goto_pos(3,32);
		OLED_print_string("Play");
		OLED_goto_pos(5,32);
		OLED_print_string("See/reset hs");		
		move_arrow(curr_arrow_pos%2);
	}
}

void GUI_menu_init(){
	
	main_menu= new_node("Main Menu", NULL);
	curr_mode= main_menu;
	play= new_node("Play", main_menu);
	sr_highscore= new_node("See/Reset Highscore", main_menu);
	calibrate= new_node("Calibrate", main_menu);
	set_diff= new_node("Set Difficulty", main_menu);
	quit= new_node("Quit", main_menu);
	
	set_child(main_menu, play, 0);
	set_child(main_menu, sr_highscore, 1);
	set_child(main_menu, calibrate, 2);
	set_child(main_menu, set_diff, 3);	
	set_child(main_menu, quit, 4);	
	
	set_fun(main_menu,GUI_main_menu);
	set_fun(play,f_play);
	set_fun(sr_highscore,f_sr_highscore);
	set_fun(calibrate,f_calibrate);
	set_fun(set_diff,f_set_diff);
	set_fun(quit,f_quit);
	GUI_init();
	OLED_print_arrow(3,15);
	//printf("CURRENT ARROW=%d\n",curr_arrow_pos);
	
}


void menu_navigation(){
	
	/*
	title_1 = prova->title;
	node * prova_parent = quit->parent;
	title_1_p = prova_parent->title;
	 printf("node : %s, parent node: %s \n", title_1, title_1_p);*/
	
	joystick = get_joypos();
	if (curr_mode->title == "Main Menu"){
		if(joystick.y_currdir == "DOWN" && curr_arrow_pos<END_MAIN_MENU){	
			
			if (curr_arrow_pos%2 ==0 ){		
				move_arrow(0);
			}
			else GUI_main_menu(((int)(curr_arrow_pos/2))+1);
			
			curr_arrow_pos=curr_arrow_pos+1;			
			}
		else if(joystick.y_currdir == "UP" && curr_arrow_pos>0){
				
			if (curr_arrow_pos%2 ==1 ){		
				move_arrow(1);		
			}
			else GUI_main_menu(((int)(curr_arrow_pos/2))-1);
			curr_arrow_pos=curr_arrow_pos-1;
		}
		
	
		//printf("CURRENT ARROW=%d\n",curr_arrow_pos);
		//printf("joystick=%d\n",joystick.y_pos);
		value = PINB;
		
		if(joystick.x_currdir == "RIGHT"){
			set_current_node(get_current_node()->child[curr_arrow_pos]);
			get_current_node()->fun_ptr();
		}
		
	}

	else{
		if(joystick.x_currdir == "LEFT"){
			set_current_node(get_current_node()->parent);
			get_current_node()->fun_ptr();
		}
			node * par_node = (get_current_node()->parent);
			char * title = par_node->title;
			char * title2 = get_current_node()->title;
			printf("parent: %s, current node: %s \n", title, title2);
	}
	//printf("%s\n",play->title);
}


	//value = PINB;	
