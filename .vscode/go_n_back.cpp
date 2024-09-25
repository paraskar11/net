#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

using namespace std;

enum ProtocolType { GoBackN, SelectiveRepeat };

class SlidingWindowProtocol {
public:
    SlidingWindowProtocol(int windowSize, int numFrames, ProtocolType protocolType)
        : windowSize(windowSize), numFrames(numFrames), protocolType(protocolType), 
          ackReceived(numFrames, false), framesSent(0), framesAcked(0) {
        srand(time(0)); // Seed random number generator
    }

    void simulate() {
        if (protocolType == GoBackN) {
            simulateGoBackN();
        } else if (protocolType == SelectiveRepeat) {
            simulateSelectiveRepeat();
        }
    }

private:
    int windowSize;                     // Size of the sliding window
    int numFrames;                      // Total number of frames to be sent
    vector<bool> ackReceived;           // Keeps track of acknowledged frames
    int framesSent;                     // Number of frames sent
    int framesAcked;                    // Number of frames acknowledged
    ProtocolType protocolType;          // The protocol type (Go-Back-N or Selective Repeat)

    bool isFrameLost() {
        return rand() % 10 < 3; // 30% probability that the frame is lost
    }

    void simulateGoBackN() {
        int base = 0; // Points to the base of the window

        while (framesAcked < numFrames) {
            // Send all frames in the window
            for (int i = base; i < base + windowSize && i < numFrames; i++) {
                if (!ackReceived[i]) {
                    cout << "Sending frame: " << i << endl;
                    framesSent++;
                    if (isFrameLost()) {
                        cout << "Frame " << i << " lost!" << endl;
                    } else {
                        cout << "Acknowledgment for frame " << i << " received." << endl;
                        ackReceived[i] = true;
                        framesAcked++;
                    }
                }
            }

            // Slide the window
            while (base < numFrames && ackReceived[base]) {
                base++;
            }

            cout << "Sliding window to base: " << base << endl;
        }

        cout << "Total frames sent (including retransmissions) in Go-Back-N: " << framesSent << endl;
    }

    void simulateSelectiveRepeat() {
        int base = 0; // Points to the base of the window

        while (framesAcked < numFrames) {
            // Send all frames in the window
            for (int i = base; i < base + windowSize && i < numFrames; i++) {
                if (!ackReceived[i]) {
                    cout << "Sending frame: " << i << endl;
                    framesSent++;
                    if (isFrameLost()) {
                        cout << "Frame " << i << " lost!" << endl;
                    } else {
                        cout << "Acknowledgment for frame " << i << " received." << endl;
                        ackReceived[i] = true;
                        framesAcked++;
                    }
                }
            }

            // Slide the window for acknowledged frames
            while (base < numFrames && ackReceived[base]) {
                base++;
            }

            cout << "Sliding window to base: " << base << endl;
        }

        cout << "Total frames sent (including retransmissions) in Selective Repeat: " << framesSent << endl;
    }
};

int main() {
    int windowSize, numFrames, protocolChoice;

    cout << "Enter window size: ";
    cin >> windowSize;

    cout << "Enter total number of frames: ";
    cin >> numFrames;

    cout << "Choose protocol (0 for Go-Back-N, 1 for Selective Repeat): ";
    cin >> protocolChoice;

    ProtocolType protocolType = (protocolChoice == 0) ? GoBackN : SelectiveRepeat;

    SlidingWindowProtocol protocol(windowSize, numFrames, protocolType);
    protocol.simulate();

    return 0;
}
