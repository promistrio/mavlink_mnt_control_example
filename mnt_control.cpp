#include <ardupilotmega/mavlink.h> // важно взять из папки с ардупилотом
// заголовочники сойдут из обычного репозитория https://github.com/mavlink/c_library_v2 

/*************************
*   Конфигурация подвеса (указать что будет внешнее управленние)
**************************/
mavlink_message_t message; 

#define MNT_SYSID   1 // id борта
#define MNT_COMPID   154 // comp id подвеса (указывается в настройках алексмоса и можно посмотреть в  мавлинк инспеторе)

#define GCS_SYSID   255 // так принято
#define GCS_COMPID   1 // так принято

mavlink_mount_configure_t mnt_configure = {0};
mnt_configure.target_system = MNT_SYSID;
mnt_configure.target_component = MNT_COMPID;
mnt_configure.mount_mode = MAV_MOUNT_MODE_MAVLINK_TARGETING; // управление мавлинком
mnt_configure.stab_roll = 1; //stabilize roll? (1 = yes, 0 = no)
mnt_configure.stab_pitch = 1; //stabilize pitch? (1 = yes, 0 = no)
mnt_configure.stab_yaw = 1; //stabilize yaw? (1 = yes, 0 = no)

mavlink_msg_mount_configure_encode(GCS_SYSID, GCS_COMPID, &message, &mnt_configure);

serial_port->write_message(message); // описание ниже


/*************************
*   задать угол
**************************/
mavlink_message_t message; 

mavlink_mount_control_t mnt_ctrl = {0};
mnt_ctrl.save_position = 0; // 1 - save to EEPROM. Если зачем-то нужно сохранять после выключения
mnt_ctrl.target_component = MNT_SYSID; // нужно подсмотреть в mavlink инспекторе на вашем подвесе
mnt_ctrl.target_system = MNT_COMPID; // единичка

// все ниже - это сотые градуса. Повернуть на 20 градусов = 2000
mnt_ctrl.input_a = 0 ; //pitch 
mnt_ctrl.input_b = 0 ; //roll
mnt_ctrl.input_c = 0 ; //yaw
//с осями желательно поиграться, они могут не соответствовать



mavlink_msg_mount_control_encode(GCS_SYSID, GCS_COMPID, &message, &mnt_ctrl);
serial_port->write_message(message); // описание ниже 

/*
* write_message
*/

int
Serial_Port::
write_message(const mavlink_message_t &message)
{
	char buf[300];

	// Translate message to buffer
	unsigned len = mavlink_msg_to_send_buffer((uint8_t*)buf, &message); // из общей структуры сообщения (mavlink_message_t) генерирует пакет

	// Write buffer to serial port, locks port while writing
	int bytesWritten = _write_port(buf,len); // _write_port будет зависеть от реализации на джетсоне

	return bytesWritten;
}