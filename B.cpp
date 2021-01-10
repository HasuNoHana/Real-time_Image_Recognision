
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

void locate(cv::Mat &img, int &x, int &y) {
    int window_row_n = 60, window_col_n = 60;
    int step = 30, sub_step = 5;
    int count, max = 0;
    int threshold_value = 242;

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
}

std::string format_message(int &x, int &y, int &Xsize, int &Ysize) {        // (to C)
    std::stringstream message;
    message << x << " ";
    message << y << " ";
    message << Xsize << " ";
    message << Ysize;
    return message.str();
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
	const int queue_id = msgget(KLUCZ_KOLEJKA, 0);
	if(queue_id==-1){
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
    cv::Mat src;
    cv::Mat dst;
    int x, y;

// This segment is extracted from the loop so that it is possible to only read the image size once.
// There is no reason to to it every time.

    // notify A that B is ready to receive the first frame
    buf.mtype = 2;
    sprintf(buf.mtext, "1");
    if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
        error_message_exit("Error while sending message to A",  shared_frame);
    }

    // wait for A to send the first frame
    if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 1, 0) == -1) {
        error_message_exit("Error while receiving message from A", shared_frame);
    }
/////////////////////////////////////////////////////////////////////////////////////////////////

    int i = 0;
    while (true) {

        // using shared memory
        src = cv::Mat(cv::Size(640, 480), CV_8UC3, shared_frame, cv::Mat::AUTO_STEP);   // use the received image
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);                                     // Convert the image to Gray
        locate(dst, x, y);                                                              // save overexposed area coordinates in x, y
//        cv::imshow("cosik",src);
//        cv::waitKey();
        //finished using shared memory

        // notify A that it can put a new frame in shmem
        buf.mtype = 2;
        strncpy(buf.mtext, "", 1);
        if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            error_message_exit("Error while sending message to A", shared_frame);
        }

        // send overexposed area coordinates to C
        buf.mtype = 3;
        strncpy(buf.mtext, format_message(x, y, dst.cols, dst.rows).c_str(), ROZMIAR_KOMUNIKATU);
        if (msgsnd(queue_id, &buf, ROZMIAR_KOMUNIKATU, 0) == -1) {
            error_message_exit("Error while sending message to C", shared_frame);
        }

        i++;                // break condition, to be replaced by receiving a message from D
        if (i >= 1) break;  //

        // wait for A to copy a new frame into shmem
        if (msgrcv(queue_id, &buf, ROZMIAR_KOMUNIKATU, 1, 0) == -1) {
            error_message_exit("Error while receiving message from A", shared_frame);
        }
    }
    // close shmem
    if (shmdt(shared_frame) == (uchar) -1) {
        std::cerr << "Error while closing shared memory: " << strerror(errno)  << std::endl;
    }
}