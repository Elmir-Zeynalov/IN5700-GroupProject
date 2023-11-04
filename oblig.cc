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
    int sent_count;
    int recv_count;
    char displayString[20];

    int cloud_to_phone;
    int cloud_to_computer;
    int cloud_sent_power;
    int clount_recv_power;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void updateCloudStats();

};

// The module class needs to be registered with OMNeT++
Define_Module(Cloud);

void Cloud::initialize() {
    sent_count = 0;
    recv_count = 0;
    cloud_to_phone = 0;
    cloud_to_computer = 0;
    cloud_sent_power = 1;
    clount_recv_power = 0;

    left = par("left").boolValue();
}

void Cloud::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "1- Contents of Book Table") == 0) {
            recv_count++;
            send(new ComputerMsg("2- ACK"), "gate$o", 1);
            cloud_to_computer++;
            scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
            send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
            cloud_to_phone++;
            sent_count+=2;
            waiting_for_ACK = true;
        } else if (strcmp(msg->getName(), "timeoutEvent") == 0) {
            if (waiting_for_ACK) {
                scheduleAt(simTime() + 1.0, new cMessage("timeoutEvent"));
                send(new ComputerMsg("3- Cloud ready to start"), "gate$o", 0);
                sent_count++;
                cloud_to_phone++;
            }
        }else if(strcmp(msg->getName(), "4- ACK") == 0){
            recv_count++;
        }

        else if (strcmp(msg->getName(), "5- Where is the book I am looking for?")
                == 0) {
            waiting_for_ACK = false;
            recv_count++;
            if (left) {
                send(new ComputerMsg("6- ACK"), "gate$o", 0);
                cloud_to_phone++;
                send(new ComputerMsg("7- The book you are looking for is in the left-hand shelf"), "gate$o", 0);
                cloud_to_phone++;
            } else {
                send(new ComputerMsg("6- ACK"), "gate$o", 0);
                cloud_to_phone++;
                send(new ComputerMsg("8- The book you are looking for is in the right-hand shelf"), "gate$o", 0);
                cloud_to_phone++;
            }
            sent_count+=2;
        }
        else if (strcmp(msg->getName(), "9- ACK")== 0) {
            recv_count++;
        }  else if (strcmp(msg->getName(), "14- Book table index") == 0) {
            recv_count++;
            sent_count++;
            send(new ComputerMsg("15- ACK"), "gate$o", 1);
            cloud_to_computer++;
            updateCloudStats();
        }
    sprintf(displayString, "sent:%d recv:%d", sent_count, recv_count);
            this->getDisplayString().setTagArg("t", 0,
                        displayString);
}

void Cloud::updateCloudStats()
{
    cCanvas* canvas = this->getParentModule()->getCanvas();
    char temp[100];

    cLabelFigure *total_num_cloud= (cLabelFigure*)(canvas->getFigure("total_num_cloud"));
    sprintf(temp, "Total number of messages sent/received by the cloud= %d", sent_count + recv_count);
    total_num_cloud->setText(temp);

    cLabelFigure *total_power_cloud_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_smart"));
    sprintf(temp, "cloud (from cloud to smartphone) = %d", cloud_to_phone * cloud_sent_power);
    total_power_cloud_to_smart->setText(temp);

    cLabelFigure *total_power_cloud_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_cloud_to_comp"));
    sprintf(temp, "cloud (from cloud to computer) = %d", cloud_to_computer * cloud_sent_power);
    total_power_cloud_to_comp->setText(temp);

}

void Cloud::finish() {
}

class Computer: public cSimpleModule {
private:
    int sent_count;
    int recv_count;
    char displayString[20];

    int computer_to_cloud;
    int computer_to_phone;

    int computer_sent_power;
    int computer_recv_power;
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void updateCloudStats();
};

// The module class needs to be registered with OMNeT++
Define_Module(Computer);

void Computer::initialize() {
    sent_count = 0;
    recv_count = 0;
    computer_to_cloud = 0;
    computer_to_phone = 0;

    computer_sent_power = 1;
    computer_recv_power = 1;

    send(new ComputerMsg("1- Contents of Book Table"), "gate$o", 0);
    computer_to_cloud++;
    sent_count++;
}

void Computer::handleMessage(cMessage *msg) {

    if (strcmp(msg->getName(), "10- Pay the Book") == 0) {
            send(new ComputerMsg("11- ACK"), "gate$o", 1);
            send(new ComputerMsg("12- Book payed"), "gate$o", 1);
            computer_to_phone+=2;
            sent_count+=2;
            bubble("Book Payed");
            send(new ComputerMsg("14- Book table index"), "gate$o", 0);
            computer_to_cloud++;
            sent_count++;
        }
    recv_count++;
    sprintf(displayString, "sent:%d recv:%d", sent_count, recv_count);
        this->getDisplayString().setTagArg("t", 0,
                    displayString);
     if(strcmp(msg->getName(), "15- ACK") == 0){
         updateCloudStats();
     }
}


void Computer::updateCloudStats()
{
    cCanvas* canvas = this->getParentModule()->getCanvas();
    char temp[100];


    cLabelFigure *total_num_comp= (cLabelFigure*)(canvas->getFigure("total_num_comp"));
    sprintf(temp, "Total number of messages sent/received by the computer= %d", sent_count + recv_count);
    total_num_comp->setText(temp);

    cLabelFigure *total_power_comp_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_comp_to_cloud"));
    sprintf(temp, "computer (from computer to cloud)= %d", computer_to_cloud * computer_sent_power);
    total_power_comp_to_cloud->setText(temp);

    cLabelFigure *total_power_comp_to_smart= (cLabelFigure*)(canvas->getFigure("total_power_comp_to_smart"));
    sprintf(temp, "computer (from computer to smartphone)= %d", computer_to_phone * computer_sent_power);
    total_power_comp_to_smart->setText(temp);
}


void Computer::finish() {
}


class MobilePhone: public cSimpleModule {
private:
    int drop_count;
    int sent_count;
    int recv_count;

    bool left;
    char displayString[20];

    int phone_to_cloud;
    int phone_to_computer;

    int phone_sent_power;
    int phone_recv_power;

protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void updateCloudStats();
};

// The module class needs to be registered with OMNeT++
Define_Module(MobilePhone);

void MobilePhone::initialize() {
    drop_count = 0;
    sent_count = 0;
    recv_count = 0;
    phone_to_cloud = 0;
    phone_to_computer = 0;

    phone_sent_power = 1;
    phone_recv_power = 1;
    left = this->getParentModule()->par("left").boolValue();
    scheduleAt(simTime() + 15.0, new cMessage("browseBook"));
    scheduleAt(simTime() + 27.0, new cMessage("payBook"));

    scheduleAt(simTime() + 55.0, new cMessage("Wait"));

}
void MobilePhone::handleMessage(cMessage *msg) {
    sprintf(displayString, "sent:%d recv:%d lost:%d", sent_count, recv_count, drop_count);
    this->getParentModule()->getDisplayString().setTagArg("t", 0,
                displayString);

    if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count < 5) {
        this->getParentModule()->bubble("Message Lost");
        drop_count++;

    } else if (strcmp(msg->getName(), "3- Cloud ready to start") == 0
            && drop_count >= 5) {
        send(new ComputerMsg("4- ACK"), "gate$o", 0);
        send(new ComputerMsg("5- Where is the book I am looking for?"),
                "gate$o", 0);
        phone_to_cloud+=2;
        sent_count+=2;
        recv_count++;
    } else if (strcmp(msg->getName(),
            "7- The book you are looking for is in the left-hand shelf") == 0
            || strcmp(msg->getName(),
                    "8- The book you are looking for is in the right-hand shelf")
                    == 0) {
        recv_count+=2;
        send(new ComputerMsg("9- ACK"), "gate$o", 0);
        phone_to_cloud++;
        sent_count++;
    } else if (strcmp(msg->getName(), "browseBook") == 0) {

        this->getParentModule()->bubble("Browse Book");
    } else if (strcmp(msg->getName(), "payBook") == 0) {

        send(new ComputerMsg("10- Pay the Book"), "gate$o", 1);
        phone_to_computer++;
        sent_count++;
    } else if (strcmp(msg->getName(), "11- ACK") == 0){
        recv_count++;
    }

    else if (strcmp(msg->getName(), "12- Book payed") == 0) {
        recv_count++;
        send(new ComputerMsg("13- ACK"), "gate$o", 1);
        phone_to_computer++;
        sent_count++;
        updateCloudStats();
    }

    sprintf(displayString, "sent:%d recv:%d lost:%d", sent_count, recv_count, drop_count);
        this->getParentModule()->getDisplayString().setTagArg("t", 0,
                    displayString);
}

void MobilePhone::updateCloudStats()
{
    //cCanvas* canvas = this->getParentModule()->getCanvas();
    cCanvas* canvas = this->getParentModule()->getParentModule()->getCanvas();
    char temp[100];


    cLabelFigure *total_num_smartphone= (cLabelFigure*)(canvas->getFigure("total_num_smartphone"));
    sprintf(temp, "Total number of messages sent/received by the smartphone= %d", sent_count + recv_count);
    total_num_smartphone->setText(temp);

    cLabelFigure *total_power_smart_to_cloud= (cLabelFigure*)(canvas->getFigure("total_power_smart_to_cloud"));
    sprintf(temp, "smartphone (from smartphone to cloud)= %d", phone_to_cloud * phone_sent_power);
    total_power_smart_to_cloud->setText(temp);

    cLabelFigure *total_power_smart_to_comp= (cLabelFigure*)(canvas->getFigure("total_power_smart_to_comp"));
    sprintf(temp, "smartphone (from smartphone to computer)= %d", phone_to_computer * phone_sent_power);
    total_power_smart_to_comp->setText(temp);

}
void MobilePhone::finish() {
}






