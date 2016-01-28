// �C���N���[�h�t�@�C���w��
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include <time.h>
#include <sys/wait.h>
//#include <unistd.h>
#include <opencv2/opencv.hpp>
//#include "colorExtaraction"
//#include <opencv/highgui.h>

#include "playSound.h"

#define SLEEP_TIME 2
#define HOUCHI_FRAME 100

#define ROCK 0
#define SCISORS 1
#define PAPER 2

#define SETTAI 1
#define ONI 2
#define MICHIEMON 3

#define ITERATIONS 3
#define COUNT_OF_DEFINITION 4
#define ACCURACY 1.5	// this number should be about 2

void cv_ColorExtraction(IplImage* src_img, IplImage* dst_img,
	int code,
	int ch1_lower, int ch1_upper,
	int ch2_lower, int ch2_upper,
	int ch3_lower, int ch3_upper
	){

	int i, k;

	IplImage *Color_img;
	IplImage *ch1_img, *ch2_img, *ch3_img;
	IplImage *Mask_img;

	int lower[3];
	int upper[3];
	int val[3];

	CvMat *lut;

	//code�Ɋ�Â����J���[�ϊ�
	Color_img = cvCreateImage(cvGetSize(src_img), src_img->depth, src_img->nChannels);
	cvCvtColor(src_img, Color_img, code);

	//3Ch��LUT�쐬
	lut = cvCreateMat(256, 1, CV_8UC3);

	lower[0] = ch1_lower;
	lower[1] = ch2_lower;
	lower[2] = ch3_lower;

	upper[0] = ch1_upper;
	upper[1] = ch2_upper;
	upper[2] = ch3_upper;

	for (i = 0; i < 256; i++){
		for (k = 0; k < 3; k++){
			if (lower[k] <= upper[k]){
				if ((lower[k] <= i) && (i <= upper[k])){
					val[k] = 255;
				}
				else{
					val[k] = 0;
				}
			}
			else{
				if ((i <= upper[k]) || (lower[k] <= i)){
					val[k] = 255;
				}
				else{
					val[k] = 0;
				}
			}
		}
		//LUT�̐ݒ�
		cvSet1D(lut, i, cvScalar(val[0], val[1], val[2]));
	}

	//3Ch���Ƃ�LUT�ϊ��i�e�`�����l�����ƂɂQ�l�������j
	cvLUT(Color_img, Color_img, lut);
	cvReleaseMat(&lut);

	//�e�`�����l�����Ƃ�IplImage���m�ۂ���
	ch1_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
	ch2_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
	ch3_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);

	//�`�����l�����Ƃɓ�l�����ꂽ�摜�����ꂼ��̃`�����l���ɕ�������
	cvSplit(Color_img, ch1_img, ch2_img, ch3_img, NULL);

	//3Ch�S�Ă�AND�����A�}�X�N�摜���쐬����B
	Mask_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
	cvAnd(ch1_img, ch2_img, Mask_img);
	cvAnd(Mask_img, ch3_img, Mask_img);

	//���͉摜(src_img)�̃}�X�N�̈���o�͉摜(dst_img)�փR�s�[����
	cvZero(dst_img);
	cvCopy(src_img, dst_img, Mask_img);

	//���
	cvReleaseImage(&Color_img);
	cvReleaseImage(&ch1_img);
	cvReleaseImage(&ch2_img);
	cvReleaseImage(&ch3_img);
	cvReleaseImage(&Mask_img);
}


int main(int argc, char **argv)
{
	srand((unsigned) time(NULL));
	int cpHand = rand() % 3;
	int playerHand;
	
	int aikoflag = 0;
	int houchiflag = 0;

	CvCapture *capture = 0;
	IplImage *frame = 0, *frame2 = 0, *tmp = 0, *frame3 = 0;
	int c, i, j, mode;
	//int sequence = 0;
	int scanNum, jFlag = 0;
	double finger;
	int f[5];

	printf("---Select Janken Mode---\n");
	printf("1:Settai\n");
	printf("2:Oni\n");
	printf("3:Michiemon\n");
	printf("ELSE:Normal\n");
	printf("Input:");
	mode = getchar() - 48;	// ???????
	printf("%d\n", mode);

	double width = 160, height = 120;
	//double width = 320, height = 240;
	//double width = 640, height = 480;
	//double width = 800, height = 600;

	// �J�����ɑ΂���L���v�`���\���̂��쐬�B
	if (argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
		capture = cvCreateCameraCapture(argc == 2 ? argv[1][0] - '0' : 0);

	// �L���v�`���T�C�Y�̐ݒ�B
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, width);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, height);

	// �E�B���h�E�쐬�B
	cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Display", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Mask", CV_WINDOW_AUTOSIZE);


	//char *command[] = {"aplay", "-D", "plughw:0,0", "/home/pi/mod/tomo/voice_wav/oc.wav", NULL};
	//execvp(command[0], command);
	playSound("/home/pi/mod/tomo/voice_wav/janken.wav", WNOHANG);
	printf("---------------------------------\n");
	sleep(100);

	while (1)
	{
		// �摜�L���v�`���B
		frame = cvQueryFrame(capture);
		frame2 = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
		//frame3 = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);
		tmp = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

		CvPoint center; center.x = (int)width / 2; center.y = (int)height * 3 / 4;
		int radius = (int)height * 3 / 8 - 1;

		//���F�̒��o
		cv_ColorExtraction(frame, frame2, CV_BGR2HSV, 0, 10, 80, 255, 0, 255);
		
		//�摜�⊮
		//�c����ITERATIONS��s��
		cvDilate(frame2, tmp, NULL, ITERATIONS);

		//���k��ITERATIONS��s��
		cvErode(tmp, frame2, NULL, ITERATIONS);
		
		//cvShowImage("Capture", frame);
		//cvShowImage("Display", frame2);


		//�摜�f�[�^�|�C���^���擾����
		uchar *p = (uchar*)frame2->imageData;
		//uchar *p3 = (uchar*)frame3->imageData;
		int num, flag, fingerNum, preNum, loopEnd;

		if (jFlag == COUNT_OF_DEFINITION - 1) {
			for (i = 0; i < 5; i++) {
				f[i] = 0;
			}
		}

		for (j = 0, finger = 0, scanNum = 0; j < height / 2; j += (int)height / 16, scanNum++) {
			num = flag = fingerNum = preNum = 0; 
			for (i = 0; i < width; i++){
				//���̓f�[�^
				//val = p[i * 3 + j * frame2->widthStep + 2];

				if (p[i * 3 + j * frame2->widthStep + 2] > 50) {
					//�ЂƂO�����F
					if (flag == 0) {
						num++;
						if (num == 3) {
							fingerNum++;
						}
					} else {//�ЂƂO�͍�
						flag = 0;
						if (num <= 2) {
							num = preNum + num + 1;
							preNum = 0;
						} else {
							preNum = num;
							num = 1;
						}
					}
					/*
					p3[i * 3 + j * frame2->widthStep] = 0;
					p3[i * 3 + j * frame2->widthStep + 1] = 0;
					p3[i * 3 + j * frame2->widthStep + 2] = 0;
					*/
				} else {
					//�ЂƂO����
					if (flag == 1) {
						num++;
						if (num == 3) {
							//fingerNum++;
						}
					} else {//�ЂƂO�͔��F
						flag = 1;
						if (num <= 2) {
							num = preNum + num + 1;
							preNum = 0;
						} else {
							preNum = num;
							num = 1;
						}
					}
					/*
					p3[i * 3 + j * frame2->widthStep] = 255;
					p3[i * 3 + j * frame2->widthStep + 1] = 255;
					p3[i * 3 + j * frame2->widthStep + 2] = 255;
					*/
				}
				//printf("%d ", p[i * 3 + j * frame2->widthStep]);
			}
			//putchar('\n');
			finger += fingerNum;
			printf("%d\n", fingerNum);
			switch (fingerNum) {
			case 0:f[0]++; break;
			case 1:f[1]++; break;
			case 2:f[2]++; break;
			case 3:f[3]++; break;
			case 4:f[4]++; break;
			}
		}
		finger /= scanNum;
		printf("\nresult:%lf\n", finger);
		putchar('\n');

		// �L���v�`�������摜�ɉ~��`��
		tmp = cvCloneImage(frame);
		cvCircle(tmp, center, radius, cvScalar(0,0,255,1));

		cvShowImage("Capture", tmp);
		cvShowImage("Display", frame2);
		//cvShowImage("Mask", frame3);

		c = cvWaitKey(2);
		if (c == '\x1b') break;	// ESC��������, break

		// �肪�Ȃ�������Acontinue
		for (i = center.x - radius, j = center.y, loopEnd = 0; i < center.x + radius; i++) {
			if (p[i * 3 + j * frame2->widthStep + 2] > 50) {
				loopEnd++;
			}
		}
		for (i = center.x - radius / 2, j = center.y - radius * 1.73 / 2; i < center.x + radius / 2; i++) {	// 1.73�̓��[�g3
			if (p[i * 3 + j * frame2->widthStep + 2] > 50) {
				loopEnd++;
			}
		}
		for (i = center.x - radius / 2, j = center.y + radius * 1.73 / 2; i < center.x + radius / 2; i++) {
			if (p[i * 3 + j * frame2->widthStep + 2] > 50) {
				loopEnd++;
			}
		}
		if (loopEnd < radius * ACCURACY) {
			//cvWaitKey(0);
			if (jFlag == 0) {
				houchiflag++;
				if (houchiflag == HOUCHI_FRAME) {
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/haraheri.wav", WNOHANG);
					
					houchiflag = 0;
				}
			} else {
				jFlag = 0;
			}
			continue;
		}
		jFlag++;
		if (jFlag == COUNT_OF_DEFINITION) {
			if (finger <= 0.7) {
				playerHand = ROCK;
				printf("your hand is ROCK!\n");
			} else if (finger <= 2) {
				if (f[4] > 0 || f[3] > 1) {
					playerHand = PAPER;
					printf("your hand is PAPER!\n");
				} else {
					playerHand = SCISORS;
					printf("your hand is SCISORS!\n");
				}
			} else {
				playerHand = PAPER;
				printf("your hand is PAPER!\n");
			}

			// print Michiemon's hand
			if (mode == SETTAI) {
				cpHand = (playerHand + 1) % 3;
			} else if (mode == ONI) {
				cpHand = (playerHand + 2) % 3;
			} else if (mode == MICHIEMON) {
				cpHand = ROCK;
			} else {
				cpHand = rand() % 3;
			}
			switch (cpHand) {
			case ROCK:
				printf("Michiemon's hand is ROCK!\n");
				break;
			case SCISORS:
				printf("Michiemon's hand is SCISORS!\n");
				break;
			case PAPER:
				printf("Michiemon's hand is PAPER!\n");
				break;
			default:
				fprintf(stderr, "Michiemon's hand is not JANKEN:%d\n", cpHand);
				exit(cpHand);
			}


			if (playerHand == cpHand) {
				printf("AIKO!!\n");
				sleep(SLEEP_TIME);
				playSound("/home/pi/mod/tomo/voice_wav/haraheri.wav",0);
				aikoflag = 1;
			} else if (playerHand == ROCK) {
				switch (cpHand) {
				case SCISORS:
					printf("YOU WIN!!\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav",0);
					break;
				case PAPER:
					printf("YOU LOSE..\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/oc_robo.wav",0);
					break;
				default:
					fprintf(stderr, "switch(hand == ROCK)-default err\n");
					exit(1);
				}
			} else if (playerHand == SCISORS) {
				switch (cpHand) {
				case PAPER:
					printf("YOU WIN!!\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav",0);
					break;
				case ROCK:
					printf("YOU LOSE..\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/oc_robo.wav",0);
					break;
				default:
					fprintf(stderr, "switch(hand == SCISORS)-default err\n");
					exit(1);
				}
			} else if (playerHand == PAPER) {
				switch (cpHand) {
				case ROCK:
					printf("YOU WIN!!\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/oc_robo.wav",0);
					break;
				case SCISORS:
					printf("YOU LOSE..\n");
					sleep(SLEEP_TIME);
					playSound("/home/pi/mod/tomo/voice_wav/oc.wav",0);
					break;
				default:
					fprintf(stderr, "switch(hand == PAPER)-default err\n");
					exit(1);
				}
			} else {
				fprintf(stderr, "your hand is not JANKEN\n");
				sleep(SLEEP_TIME);
				playSound("/home/pi/mod/tomo/voice_wav/sayonara.wav",0);
				exit(1);
			}

			cvWaitKey(0);
			if (aikoflag != 0) {
				playSound("/home/pi/mod/tomo/voice_wav/aiko.wav",WNOHANG);
				aikoflag = 0;
			} else {
				playSound("/home/pi/mod/tomo/voice_wav/janken.wav",WNOHANG);
			}
		}
	}
	// ��Еt���B
	cvReleaseCapture(&capture);
	cvDestroyWindow("Capture");
	cvDestroyWindow("Display");
	//cvDestroyWindow("Mask");

	return 0;
}
