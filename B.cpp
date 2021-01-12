
#include <iostream>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include "dane.hpp"

bool locate(cv::Mat &img, int &x, int &y) {
    int window_row_n = 60, window_col_n = 60;
    int step = 30, sub_step = 5;
    int count, max = 0;
    int threshold_value = 242;

    x = 0;
    y = 0;

    for (int row = 0; row < img.rows - window_row_n; row += step) {         // move sliding window
        for (int col = 0; col < img.cols - window_col_n; col += step) {     //
            count = 0;
            for (int k = 0; k < window_row_n; k += sub_step) {                          // probe sliding window
                for (int l = 0; l < window_col_n; l += sub_step) {                      //
                    if (img.at<uchar>(row + k, col + l) >= threshold_value) count++;    //
                }
            }
            if (count > max) {                          // there are more white pixels than ever before
                x = col + window_col_n/2;
                y = row + window_row_n/2;
                max = count;
            }
        }
    }
    if (max > 0) return true;
    else return false;
}

inline void error_message_exit(const char *message, uchar *shared_frame) {
    std::cerr << message << ": " << strerror(errno)  << std::endl;
    if(shmdt(shared_frame)==-1){
	    std::cerr << "Error while closing shared memory: " << strerror(errno) << std::endl;
	}
    exit(1);
}

int main(int argc, char** argv) {
    
    // open message queue
	const int queue_id_1 = msgget(KLUCZ_KOLEJKA_1, 0);
	if(queue_id_1==-1){
		error_message_exit("Error while opening message queue", nullptr);
	}
    const int queue_id_2 = msgget(KLUCZ_KOLEJKA_2, 0);
	if(queue_id_2==-1){
		error_message_exit("Error while opening message queue", nullptr);
	}

	// open shared memory 
	const int shmem_id = shmget(KLUCZ_PAMIEC, 0, 0);
	if(shmem_id==-1){
		error_message_exit("Error while opening shared memory", nullptr);
	}

    // attaching shared memory segment
    uchar *shared_frame = (uchar *)shmat(shmem_id, NULL, 0);
    if (shared_frame == (uchar *) -1) {
        error_message_exit("Error while attaching shared memory segment", nullptr);
    }

    bufmsg buf;
    bufmsg_txt buf_txt;
    cv::Mat src;
    cv::Mat dst;
    int x, y;

    // notify A that B is ready to receive the first frame
    buf_txt.mtype = 3;
    sprintf(buf_txt.mtext, "1");
    if (msgsnd(queue_id_1, &buf_txt, ROZMIAR_KOMUNIKATU, 0) == -1) {
        error_message_exit("Error while sending message to A",  shared_frame);
    }

    int i = 0;
    while (true) {
        // wait for A to send the first frame or D to terminate the whole program 
        if (msgrcv(queue_id_1, &buf_txt, ROZMIAR_KOMUNIKATU, -2, 0) == -1) {
            error_message_exit("Error while receiving message from A", shared_frame);
        }
        if (buf_txt.mtype == 2) break;
        // std::cerr << "A -> B" << std::endl;

        bool controller_in_range = false;

        // using shared memory
        src = cv::Mat(cv::Size(640, 480), CV_8UC3, shared_frame, cv::Mat::AUTO_STEP);   // use the received image
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);                                     // convert the image to gray
        controller_in_range = locate(dst, x, y);                                        // save overexposed area coordinates in x, y
        //finished using shared memory

        // std::cerr << "orig x: " << x;
        // std::cerr << " y: " << y << std::endl;

        // notify A that it can put a new frame in shmem
        buf_txt.mtype = 3;
        // strncpy(buf.mtext, "", 1);
        if (msgsnd(queue_id_1, &buf_txt, ROZMIAR_KOMUNIKATU, 0) == -1) {
            error_message_exit("Error while sending message to A", shared_frame);
        }


        if (controller_in_range) {
            // send overexposed area coordinates to C
            buf.mtype = 1;
            buf.mtext[0] = x;
            buf.mtext[1] = y;
            buf.mtext[2] = dst.cols;
            buf.mtext[3] = dst.rows;

            // std::cerr << " x: " << buf.mtext[0];
            // std::cerr << " y: " << buf.mtext[1];
            // std::cerr << " length: " << buf.mtext[2];
            // std::cerr << " height: " << buf.mtext[3] << std::endl;

            // strncpy(buf.mtext, format_message(x, y, dst.cols, dst.rows).c_str(), ROZMIAR_KOMUNIKATU);
            if (msgsnd(queue_id_2, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
                error_message_exit("Error while sending message to C", shared_frame);
            }
        }
    }
    // close shmem
    if (shmdt(shared_frame) == (uchar) -1) {
        std::cerr << "Error while closing shared memory: " << strerror(errno)  << std::endl;
    }
    // std::cerr << "D -> B" << std::endl;
}