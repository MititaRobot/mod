#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include "/usr/include/linux/i2c-dev.h"

// 怒りのグローバル
int onpa_fd2, onpa_fd4, onpa_fd6, onpa_fd8;

void onpa_init()
{
  const char filename[] = "/dev/i2c-1";
  onpa_fd2 = open(filename, O_RDWR);
  onpa_fd4 = open(filename, O_RDWR);
  onpa_fd6 = open(filename, O_RDWR);
  onpa_fd8 = open(filename, O_RDWR);
  if (onpa_fd2 < 0) {
    fprintf(stderr, "0xE2 Error\n");
    exit(1);
  }
  if (onpa_fd4 < 0) {
    fprintf(stderr, "0xE4 Error\n");
    exit(1);
  }
  if (onpa_fd6 < 0) {
    fprintf(stderr, "0xE6 Error\n");
    exit(1);
  }
  if (onpa_fd8 < 0) {
    fprintf(stderr, "0xE8 Error\n");
    exit(1);
  }
  if (ioctl(onpa_fd2, I2C_SLAVE, (0xE2 >> 1)) < 0) {  // 0xE0(0x11100000) >> 1 = 0x70(0x01110000)
    fprintf(stderr, "Error on slave address 0xE2\n" );
    exit(1);
  }
  if (ioctl(onpa_fd4, I2C_SLAVE, (0xE4 >> 1)) < 0) {  // 0xE0(0x11100000) >> 1 = 0x70(0x01110000)
    fprintf(stderr, "Error on slave address 0xE4\n" );
    exit(1);
  }
  if (ioctl(onpa_fd6, I2C_SLAVE, (0xE6 >> 1)) < 0) {  // 0xE0(0x11100000) >> 1 = 0x70(0x01110000)
    fprintf(stderr, "Error on slave address 0xE6\n" );
    exit(1);
  }
  if (ioctl(onpa_fd8, I2C_SLAVE, (0xE8 >> 1)) < 0) {  // 0xE0(0x11100000) >> 1 = 0x70(0x01110000)
    fprintf(stderr, "Error on slave address 0xE8\n" );
    exit(1);
  }
}

void onpa_close()
{
  close(onpa_fd2);
  close(onpa_fd4);
  close(onpa_fd6);
  close(onpa_fd8);
}

// id は 2, 4, 6, 8のみ対応　シールと対応してるはず
int get_onpa_length(int id)
{
  char buf[10];
  // read from id
  int fd;
  int range;
  if (id == 2)
    fd = onpa_fd2;
  else if (id == 4)
    fd = onpa_fd4;
  else if (id == 6)
    fd = onpa_fd6;
  else if (id == 8)
    fd = onpa_fd8;
  else
    fprintf(stderr, "Invalid input. ID: %d", id);

  // コマンドレジスタ0に 0x51:Real Ranging Mode - Result in centimeters を送ることによって測距が始まる
  buf[0] = 0x00;
  buf[1] = 0x51;

  if ((write(fd, buf, 2)) != 2) {
    fprintf(stderr, "0x%d Error send the read command\n", id);
    exit(1);
  }

  // Wait for the measurement
  // 音波だから、"行って" "帰ってくる" のに時間がかかる
  usleep(66000);

  // コマンドレジスタ2に対し測距値データの上位バイトをリクエスト
  buf[0] = 0x02;
  if ((write(fd, buf, 1)) != 1) {
    fprintf(stderr, "0x%d Error send the read command\n", id);
    exit(1);
  }

  if ((read(fd, buf, 1)) != 1 ) {
    fprintf(stderr, "0x%d Error send the read command\n", id);
    exit(1);
  }
  range = buf[ 0 ] << 8;

  // コマンドレジスタ3に対し測距値データの下位バイトをリクエスト
  buf[0] = 0x03;
  if ((write(fd, buf, 1)) != 1) {
    fprintf(stderr, "0x%d Error send the read command\n", id);
    exit(1);
  }

  if ((read( fd, buf, 1)) != 1) {
    fprintf(stderr, "0x%d Error send the read command\n", id);
    exit(1);
  }

  // 上位と下位をくっつける
  range |= buf[0];

  printf("0xE0 Range=%d cm\n", range);
  return range;
}

