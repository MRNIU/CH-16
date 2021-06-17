
// This file is a part of MRNIU/CH-16
// (https://github.com/MRNIU/CH-16).
//
// ch16.c for MRNIU/CH-16.

#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "string.h"
#include "stdio.h"
#include "ch16.h"

static uint32_t set_msop_buf(int sock, msop_t *msop_buf) {
    struct sockaddr_in peeraddr;
    socklen_t          peerlen;
    int                n;
    peerlen = sizeof(peeraddr);
    bzero(msop_buf, 1248);
    n = recvfrom(sock, msop_buf, 1248, 0, (struct sockaddr *)&peeraddr,
                 &peerlen);
    if (n <= 0) {
        if (errno != EINTR) {
            printf("recvfrom error");
        }
    }
    else if (n > 0) {
        sendto(sock, msop_buf, n, 0, (struct sockaddr *)&peeraddr, peerlen);
    }
    close(sock);
    return 0;
}

#define PORT 2368

uint32_t get_msop(msop_t *_msop_buf) {
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error");
    }
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("监听 %d 端口\n", PORT);
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind error");
    }
    set_msop_buf(sock, _msop_buf);
    return 0;
}

uint32_t get_data(msop_t *_msop, data_t *_data_buf) {
    double   dec = 1 / 256;
    uint16_t tmp = 0;
    for (uint32_t i = 0; i < 171; i++) {
        _data_buf[i].angle.vertical = _msop->subframe[i].line;
        tmp                         = _msop->subframe[i].horizontal << 8;
        tmp += _msop->subframe[i].horizontal & 0xFF;
        _data_buf[i].angle.horizontal = tmp / 100;
        // _data_buf[i].angle.horizontal = _msop->subframe[i].horizontal / 100;
        tmp = _msop->subframe[i].distance << 8;
        tmp += _msop->subframe[i].distance & 0xFF;
        _data_buf[i].distance = tmp + (dec * _msop->subframe[i].decimal);
        // _data_buf[i].distance =
        // _msop->subframe[i].distance + (dec * _msop->subframe[i].decimal);
        _data_buf[i].strength = _msop->subframe[i].strength;
    }
    return 0;
}

uint32_t get_vertical(data_t *_data, uint8_t _line) {
    if (_data->angle.vertical == _line) {
        return 0;
    }
    else {
        return 1;
    }
}

uint32_t get_horizontal(data_t *_data, double _horizontale) {
    if (_data->angle.horizontal == _horizontale) {
        return 0;
    }
    else {
        return 1;
    }
}

uint32_t get_vertical_horizontal(data_t *_data, uint8_t _line,
                                 double _horizontale) {
    if ((get_horizontal(_data, _horizontale) == 0) &&
        (get_horizontal(_data, _line) == 0)) {
        return 0;
    }
    else {
        return 1;
    }
}

double get_min_distance(uint8_t _line, double _horizontale) {
    msop_t *msop = malloc(1248);
    data_t *data = malloc(171 * sizeof(data_t));
    // 计数
    uint8_t count = 0;
    // 最小值
    double distance_min = -1;
    while (1) {
        if (get_msop(msop) == 0) {
            // 判断是否转了一圈
            if ((msop->subframe[0].line == 0xFF) &&
                (msop->subframe[0].horizontal == 0xAABB) &&
                (msop->subframe[0].distance == 0xCCDD)) {
                // 第二次回到原点时返回
                if (count++ == 2) {
                    free(msop);
                    free(data);
                    return distance_min;
                }
            }
            // 获取数据
            if (get_data(msop, data) == 0) {
                // 如果这组数据在指定区域内
                if (get_vertical_horizontal(data, _line, _horizontale) == 0) {
                    // 更新最小值
                    if (data->distance < distance_min) {
                        distance_min = data->distance;
                    }
                }
            }
        }
    }
    return 0;
}
