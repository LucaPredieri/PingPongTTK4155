#define K_P 0.5
#define K_I 1
#define K_D 10
#define MOTOR_FREQ 50
#define PERIOD (1/MOTOR_FREQ)

typedef struct {

	float K_p;
	float K_i;
	uint16_t K_d;
	uint16_t T;
	uint16_t total_err;
	uint16_t curr_err;
	uint16_t prev_err;
	}PID_t;




void PID_init(PID_t *pid, float k_p,uint16_t k_i,uint16_t k_d, uint16_t t){	
	pid->K_p=k_p;
	pid->K_i=k_i;
	pid->K_d=k_d;
	pid->T=t;
	pid->prev_err=0;
	pid->total_err=0;
	pid->curr_err=0;
}

uint16_t PID_controller(PID_t *pid, uint16_t curr_pos, uint16_t des_pos){
	pid->curr_err=curr_pos-des_pos;
	pid->total_err+=pid->curr_err;
	uint16_t u_p=(pid->K_p)*(pid->curr_err);
	uint16_t u_i=PERIOD*K_I*(pid->total_err);
	uint16_t u_d=K_D/(pid->T)*((pid->curr_err)-(pid->prev_err));
	pid->prev_err=pid->curr_err;
	uint16_t u=u_p+u_i+u_d;
	return u;	
}