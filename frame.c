#include "frame.h"

const uint16_t crc16_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

// 计算CRC16校验值的函数
static uint16_t calculate_crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0x0;  // 初始值为0x0

    for (size_t i = 0; i < len; i++) {
        uint8_t index = ((uint8_t)(crc >> 8) ^ data[i]) & 0xFF;
        crc = (crc << 8) ^ crc16_table[index];
    }

    return crc;
}


INT32 COM_AssembleExtFrame(UINT8 * pbBuf, INT32 iBufLen) {
    UINT16 i = 0;
    INT32 iRecvLen = 0;
    UINT16 wCRCCode = 0;
    UINT8 bBuffer[EXT_FRAME_MAX_LENGTH];

    if ((iBufLen > EXT_DATA_MAX_LENGTH) || (iBufLen <= 0)) {
        iRecvLen = -1;
    } else {
        //判断是否需添加转义符
        for (i = 0; i < (iBufLen - 1); i++) {
            //如果连续两个字节为0x5A和0xFE，则在其前添加一个字节（值为0）
            if ((COM_ZW1 == pbBuf[i]) && (COM_ZW2 == pbBuf[i + 1])) {
                bBuffer[iRecvLen] = 0;
                iRecvLen++;
            }

            bBuffer[iRecvLen] = pbBuf[i];
            iRecvLen++;

        }

        //拷贝最后一个字节数据
        bBuffer[iRecvLen] = pbBuf[iBufLen - 1];
        iRecvLen++;

        wCRCCode = calculate_crc16(bBuffer, iRecvLen); //计算CRC校验码，转义后计算

        //添加帧头
        pbBuf[0] = COM_ZT1;
        pbBuf[1] = COM_ZT2;

        //添加转义后的数据
        memcpy(&pbBuf[2], bBuffer, iRecvLen);

        //添加校验和
        pbBuf[iRecvLen + 2] = COM_TC;
        pbBuf[iRecvLen + 3] = (UINT8) (wCRCCode & 0xff);
        pbBuf[iRecvLen + 4] = (UINT8) (wCRCCode >> 8);

        //添加数据长度
        pbBuf[iRecvLen + 5] = COM_TC;
        pbBuf[iRecvLen + 6] = (UINT8) (iRecvLen & 0xff);
        pbBuf[iRecvLen + 7] = (UINT8) (iRecvLen >> 8);

        //添加帧尾
        pbBuf[iRecvLen + 8] = COM_ZWTC;
        pbBuf[iRecvLen + 9] = COM_ZW1;
        pbBuf[iRecvLen + 10] = COM_ZW2;

        //计算返回值
        iRecvLen = iRecvLen + 11;
    }

    return iRecvLen;
}

INT32 COM_ParseExtFrame(uint8_t * pbBuf, int wBufLen) {
    INT32 iResult = 0;
    INT32 i, iRecvLen = 0;
    uint8_t wLen = 0;
    int iZTpos = 0;
    int wCRCCode = 0;

    if ((COM_ZW2 != pbBuf[wBufLen - 1]) || (COM_ZW1 != pbBuf[wBufLen - 2]) || (COM_ZWTC != pbBuf[wBufLen - 3])) {
        //帧尾校验错误
        iResult = -1;
    }

    else {
        wLen = pbBuf[wBufLen - 4];
        wLen = (wLen << 8) | (pbBuf[wBufLen - 5]);
//		printf("wlen=%d\n",wLen);
        iZTpos = wBufLen - 11 - wLen;
//		printf("iZTpos=%d\n",iZTpos);
        if ((iZTpos >= 0) && ((COM_TC == pbBuf[wBufLen - 6]))) {
//			printf("%x %x\n",pbBuf[iZTpos],pbBuf[iZTpos+1]);
            if ((pbBuf[iZTpos] != COM_ZT1) || (pbBuf[iZTpos + 1] != COM_ZT2)) {
                //帧头校验错误
                iResult = -2;
            } else {
                wCRCCode = calculate_crc16(((uint8_t*) &pbBuf[iZTpos + 2]), wLen); //计算CRC校验码
//				printf("wCRCCode=%04x\n",wCRCCode);
//				printf("%x %x\n",pbBuf[iZTpos + 2],wLen);
                if ((pbBuf[wBufLen - 7] == (wCRCCode >> 8)) && (pbBuf[wBufLen - 8] == (wCRCCode & 0xff)) && (COM_TC == pbBuf[wBufLen - 9])) {
//					printf("code %x\n",wCRCCode);
                    //寻找转义符
                    for (i = iZTpos + 2; i < (wBufLen - 9); i++) {
                        if ((COM_ZW1 == pbBuf[i + 1]) && (COM_ZW2 == pbBuf[i + 2])) {
                            if ((0x0 != pbBuf[i])) {
                                //转义错误
                                iResult = -3;
                                break;
                            }
                        } else {
                            //pbBuf[i]不是转义符
                            pbBuf[iRecvLen] = pbBuf[i];
                            iRecvLen++;
                        }
                    }

                    // 解帧过程中未发现转义错误，则返回数据长度
                    if (iResult != -3) {
                        iResult = iRecvLen;
                    }
                } else {
                    //校验和校验错误
                    iResult = -4;
                }
            }
        } else {
            //长度校验错误
            iResult = -5;
        }
    }
    return iResult;
}
