#include "VOFA_FUNCTIONS_WYH.H"
#include "UART.H"

JF_Frame JustFloat_Data;
extern VOFA_CMD cmd;

void VOFA_Justfloat_Init_wyh(void){
	cmd.cmd_ID = INVALID; 
	cmd.cmd_Type = INVALID;
	cmd.Completion_Flag = 0;
	JustFloat_Data.frametail[0] = 0x00;
	JustFloat_Data.frametail[1] = 0x00;
	JustFloat_Data.frametail[2] = 0x80;
	JustFloat_Data.frametail[3] = 0x7f;
}

void VOFA_Send_Justfloat_CH_wyh(float *CH_data){
	printf_float_wyh(CH_data);
}

void VOFA_Send_Justfloat_frametail(uint8_t *Array){
	uart_sendbytes_wyh(Array,FRAME_TAIL_SIZE);
}

void printf_float_wyh(float *float_data){
	uint8_t u8Array[4];
	float2uint8_tArray(u8Array,float_data,1);
	uart_sendbytes_wyh(u8Array,sizeof(u8Array));
}

void VOFA_Send_Firewater_wyh(float fdata){
	printf("%.6f\n",fdata);
}

void VOFA_Send_Rawdata_wyh(uint8_t Data){
		uart_sendbyte(Data);
}

void VOFA_translate_cmd_wyh(void){
	uint8_t *Ptr_to_Rx_Packet;
	Ptr_to_Rx_Packet = cmd.uart_Rx_Packet;
	
	if (cmd.uart_Rx_Packet[0] != '@' ||cmd.uart_Rx_Packet[3] != '=' || cmd.uart_Rx_Packet[CMD_FRAME_SIZE - 2] != '!' ||cmd.uart_Rx_Packet[CMD_FRAME_SIZE - 1] != '#'){
		memset(cmd.uart_Rx_Packet, 0, CMD_FRAME_SIZE);
		return;
	}
	
	switch (cmd.uart_Rx_Packet[1]) {
		case 'S':
			cmd.cmd_Type = Speed;
			break;
		case 'P':
			cmd.cmd_Type = Position;
			break;
		default:
			cmd.cmd_Type = INVALID;
			break;
	}
	
	switch (cmd.uart_Rx_Packet[2]) {
		case '1':
			cmd.cmd_ID = Direct_Assignment;
			break;
		case '2':
			cmd.cmd_ID = Increase;
			break;
		case '3':
			cmd.cmd_ID = Decrease;
			break;
		default:
			cmd.cmd_ID = INVALID;
			break;
	}
	memcpy(cmd.Valid_Data, (Ptr_to_Rx_Packet + 4), 4);
	
	cmd.float_data = uint8_tArray2float(cmd.Valid_Data,1);
	
	Ptr_to_Rx_Packet = NULL;
	memset(cmd.Valid_Data, 0, 4);
	memset(cmd.uart_Rx_Packet, 0, CMD_FRAME_SIZE);
}