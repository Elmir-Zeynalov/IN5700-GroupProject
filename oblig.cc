//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "ComputerMessage_m.h"

using namespace omnetpp;


class Cloud: public cSimpleModule {
private:
    bool waiting_for_ACK;
    bool left;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Cloud);

void Cloud::initialize() {
    left = par("left").boolValue();
}

void Cloud::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
            send(new ComputerMsg("2- ACK"), "gate$o", 1);
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
            waiting_for_ACK = true;
        } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
            if (waiting_for_ACK) {
                scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
                send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
            }
        } else if (strcmp(msg->getName(), "5- Where is the book I am looking for?")
                == 0) {
            waiting_for_ACK = false;
            //if (left) {
                send(new ComputerMsg("6- ACK"), "gate$o", 0);
                send(new ComputerMsg("7- The book you are looking for is in the left-hand shelf"), "gate$o", 0);
            //} else {
                //send(new ComputerMsg("6- ACK"), "gate$o", 0);
                //send(new ComputerMsg("8- The book you are looking for is in the right-hand shelf"), "gate$o", 0);
            //}
        } else if (strcmp(msg->getName(), "14- Book table index") == 0) {
            send(new ComputerMsg("15- ACK"), "gate$o", 1);
        }

}

void Cloud::finish() {
}

class Computer: public cSimpleModule {
private:

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
}

void Computer::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
            send(new ComputerMsg("11- ACK"), "gate$o", 1);
            send(new ComputerMsg("12- Book payed"), "gate$o", 1);
            send(new ComputerMsg("14- Book table index"), "gate$o", 0);
        }
}

void Computer::finish() {
}


class Host: public cSimpleModule {
private:
    int drop_count;
    bool left;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

};

// The module class needs to be registered with OMNeT++
Define_Module(Host);

void Host::initialize() {
    drop_count = 5;
    //left = this->getParentModule()->par("left").boolValue();
    scheduleAt(simTime() + 14.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 27.0, new cMessage("payBook"));

    scheduleAt(simTime() + 55.0, new cMessage("Wait"));
}
void Host::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count < 5) {
        this->getParentModule()->bubble("Message Lost");
        drop_count++;
    } else if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count >= 5) {
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        send(new ComputerMsg("5- Where is the book I am looking for?"),
                "gate$o", 0);
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the left-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the right-hand shelf")
                    == 0) {
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMsg("10- Pay the Book"), "gate$o", 1);
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMsg("13- ACK"), "gate$o", 1);
    }
}
/*
void Mobile::handleMessage(cMessage *msg) {
    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count > 0) {
       bubble("Message Lost");
        drop_count--;
    } else if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count <= 0) {
        send(new ComputerMessage("4- ACK"), "outCloud");
        send(new ComputerMessage("5- Where is the book I am looking for?"),
                "outCloud");
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the right-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the left-hand shelf")
                    == 0) {
        send(new ComputerMessage("9- ACK"), "outCloud");
    } else if (strcmp(msg->getName(), "browseBook") == 0) {
        bubble("Browse Book");
    } /*else if (strcmp(msg->getName(), "payBook") == 0) {
        send(new ComputerMessage("10- Pay the Book"), "outCloud", 1);
    } else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        send(new ComputerMessage("13- ACK"), "outCloud", 1);
    }
    */
//}

void Host::finish() {
}







