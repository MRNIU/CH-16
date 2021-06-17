
// This file is a part of MRNIU/CH-16
// (https://github.com/MRNIU/CH-16).
//
// ch16.h for MRNIU/CH-16.

#ifndef _CH16_H_
#define _CH16_H_

#include "stdint.h"

// 获取距离
// uint16_t get_distance(void) const {
//     uint16_t tmp = distance << 8;
//     tmp += distance & 0xFF;
//     return tmp * 2;
// }

// // 获取方位角
// uint16_t get_azimuth(void) const {
//     uint16_t tmp = azimuth << 8;
//     tmp += azimuth & 0xFF;
//     return tmp / 100.0;
// }

// udp 包头
typedef struct {
    // ethernet mac
    uint8_t mac[12];
    // ethernet 类型
    uint16_t type;
    // 网络协议
    uint8_t internet_protocol[20];
    // 源端口
    uint16_t src_port;
    // 目标端口
    uint16_t desc_port;
    // 长度，固定为 1214，0x04BE
    uint16_t len;
    // 校验和
    uint16_t check_sum;
} __attribute__((packed)) header_t;

// 子帧
typedef struct {
    // 线号
    uint8_t line;
    // 水平角度，高位在前，单位 0.01 度
    uint16_t horizontal;
    // 距离，高位在前，单位厘米
    uint16_t distance;
    // 距离，小数部分，单位 1/256 厘米
    uint8_t decimal;
    // 强度
    uint8_t strength;
} __attribute__((packed)) subframe_t;

// 数据包，大小为 1206 字节
typedef struct {
    // 包头
    header_t header;
    // 数据帧
    subframe_t subframe[171];
    // 预留
    uint8_t reserved[3];
    // 时间戳，单位微秒
    uint32_t timestemp;
    // 厂商信息
    uint16_t factory;
} __attribute__((packed)) msop_t;

// 角度信息
typedef struct {
    // 垂直线号
    uint8_t vertical;
    // 水平角度
    double horizontal;
} angle_t;

// 处理好的数据
typedef struct {
    // 角度信息
    angle_t angle;
    // 距离
    double distance;
    // 强度
    uint8_t strength;
} data_t;

// 通过 udp 读取 msop_t
// 返回: 成功返回 0
// _data_buf: 接收 data 的缓冲区
uint32_t get_msop(msop_t *_msop_buf);
// 获取一个数据包的距离与强度信息
// 返回: 成功返回 0
// _msop: 要解析的数据
// _data_buf: 接收 data 的缓冲区
uint32_t get_data(msop_t *_msop, data_t *_data_buf);
// 仅获取指定线号的数据
// 返回: 成功返回 0
// _data: 要解析的数据
// _line: 需要的线号
uint32_t get_vertical(data_t *_data, uint8_t _line);
// 仅获取指定水平角度的数据
// 返回: 成功返回 0
// _data: 要解析的数据
// _horizontal: 需要的角度
uint32_t get_horizontal(data_t *_data, double _horizontale);
// 获取指定 line 与 horizontal 的数据
// 返回: 成功返回 0
// _data: 要解析的数据
// _line: 需要的线号
// _horizontal: 需要的角度
uint32_t get_vertical_horizontal(data_t *_data, uint8_t _line,
                                 double _horizontale);
// 获取指定区域距离最小值
// _data: 要解析的数据
// _line: 需要的线号
// _horizontal: 需要的角度
double get_min_distance(uint8_t _line, double _horizontale);

#endif
