#include "UART_WYH.H"
#include "UART.H"

uint8_t Rx_count = 0;
VOFA_CMD cmd;

void uart_sendbytes_wyh(uint8_t *Array,uint8_t SIZE){
	while (SIZE){
		 uart_sendbyte(*Array);
     Array++;
     SIZE--;
	}
}

void uart_cmd_received(uint8_t byte_data){
	cmd.uart_Rx_Packet[Rx_count] = byte_data;
	
	if (cmd.uart_Rx_Packet[Rx_count - 1] == '!' && cmd.uart_Rx_Packet[Rx_count] == '#'){
		cmd.Completion_Flag = 1;
		Rx_count = 0;
	}
	
	else if (Rx_count > (CMD_FRAME_SIZE - 1)){
		cmd.Completion_Flag = 0;
		Rx_count = 0;
		memset(cmd.uart_Rx_Packet, 0, 10);
	}
	
	else{
		Rx_count++;
	}
}