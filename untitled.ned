//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
import inet.mobility.contract.IMobility;
import inet.mobility.single.TurtleMobility;
import inet.visualizer.integrated.IntegratedVisualizer;

module MobileHost
{
    parameters:
        bool left = default(false);
        @display("i=device/pocketpc"); // add a default icon
    gates:
        inout gate[2];
    submodules:
        mobility: <default("StationaryMobility")> like IMobility {
            @display("p=100,100;is=s");
        }
        host: MobilePhone;
    connections:
        host.gate[0] <--> gate[0];
        host.gate[1] <--> gate[1];
}

simple Computer
{
    parameters:
        @display("i=device/pc"); // add a default icon
    gates:
        inout gate[2];
}


simple MobilePhone
{
    parameters:
        @display("i=misc/cloud"); 
    gates:
        inout gate[2];
}

simple Cloud
{
    parameters:
        bool left = default(false);
        @display("i=misc/cloud"); // add a default icon
    gates:
        inout gate[2];
}



network FogNetwork
{
    @display("bgb=1240.448,699.776");
    @figure[Library](type=rectangle; pos=450,200; size=600,350; fillColor=gray; fillOpacity=0.2);
    @figure[Library.label](type=text; pos=460,210; text=Library);

    @figure[Pillar_1](type=rectangle; pos=650,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Shelf_1](type=rectangle; pos=610,300; size=40,250; fillColor=white; fillOpacity=0.8);


    @figure[Pillar_2](type=rectangle; pos=840,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Shelf_2](type=rectangle; pos=860,300; size=40,250; fillColor=white; fillOpacity=0.8);


    @figure[total_num_cloud](type=text; pos=18,100; text=Total number of messages sent/received by the cloud: N/A);
    @figure[total_num_comp](type=text; pos=18,115; text=Total number of messages sent/received by the computer: N/A);
    @figure[total_num_smartphone](type=text; pos=18,130; text=Total number of messages sent/received by the smartphone: N/A);

    @figure[total_power](type=text; pos=18,160; text=Total power spent when sending messages by:);
    @figure[total_power_cloud_to_smart](type=text; pos=18,175; text=cloud (from cloud to smartphone): N/A);
    @figure[total_power_cloud_to_comp](type=text; pos=18,190; text=cloud (from cloud to computer): N/A);
    @figure[total_power_comp_to_cloud](type=text; pos=18,205; text=computer (from computer to cloud): N/A);
    @figure[total_power_comp_to_smart](type=text; pos=18,220; text=computer (from computer to smartphone): N/A);
    @figure[total_power_smart_to_cloud](type=text; pos=18,235; text=smartphone (from smartphone to cloud): N/A);
    @figure[total_power_smart_to_comp](type=text; pos=18,250; text=smartphone (from smartphone to computer): N/A);

    @figure[total_power_rcvd](type=text; pos=18,280; text=Total power spent when receiving messages by:);
    @figure[total_power_rcvd_cloud_to_smart](type=text; pos=18,295; text=cloud (from cloud to smartphone): N/A);
    @figure[total_power_rcvd_cloud_to_comp](type=text; pos=18,310; text=cloud (from cloud to computer): N/A);
    @figure[total_power_rcvd_comp_to_smart](type=text; pos=18,325; text=computer (from computer to smartphone): N/A);
    @figure[total_power_rcvd_comp_to_cloud](type=text; pos=18,340; text=computer (from computer to cloud): N/A);
    @figure[total_power_rcvd_smart_to_cloud](type=text; pos=18,355; text=smartphone (from smartphone to cloud): N/A);
    @figure[total_power_rcvd_smart_to_comp](type=text; pos=18,370; text=smartphone (from smartphone to computer): N/A);

    @figure[total_delay](type=text; pos=18,400; text=Total delay when sending messages by:);
    @figure[total_delay_cloud_to_smart](type=text; pos=18,415; text=cloud (from cloud to smartphone): N/A);
    @figure[total_delay_cloud_to_comp](type=text; pos=18,430; text=cloud (from cloud to computer): N/A);
    @figure[total_delay_comp_to_smart](type=text; pos=18,445; text=computer (from computer to smartphone): N/A);
    @figure[total_delay_comp_to_cloud](type=text; pos=18,460; text=computer (from computer to cloud): N/A);
    @figure[total_delay_smart_to_cloud](type=text; pos=18,475; text=smartphone (from smartphone to cloud): N/A);
    @figure[total_delay_smart_to_comp](type=text; pos=18,490; text=smartphone (from smartphone to computer): N/A);

    @figure[total_delay_rcvd](type=text; pos=18,520; text=Total delay when receiving messages by:);
    @figure[total_delay_rcvd_cloud_to_smart](type=text; pos=18,535; text=cloud (from cloud to smartphone): N/A);
    @figure[total_delay_rcvd_cloud_to_comp](type=text; pos=18,550; text=cloud (from cloud to computer): N/A);
    @figure[total_delay_rcvd_comp_to_smart](type=text; pos=18,565; text=computer (from computer to smartphone): N/A);
    @figure[total_delay_rcvd_comp_to_cloud](type=text; pos=18,580; text=computer (from computer to cloud): N/A);
    @figure[total_delay_rcvd_smart_to_cloud](type=text; pos=18,595; text=smartphone (from smartphone to cloud): N/A);
    @figure[total_delay_rcvd_smart_to_comp](type=text; pos=18,610; text=smartphone (from smartphone to computer): N/A);


    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        cloud: Cloud {
            @display("p=1200.808,49.280003;r=600");
        }
        host: MobileHost {
            @display("p=750.0,520;r=80");
        }
        computer: Computer {
            @display("p=750.0,310.0;r=170");
        }
        visualizer: IntegratedVisualizer {
            @display("p=50,50");
        }

    connections:
        host.gate[0] <--> Channel <--> cloud.gate[0];
        host.gate[1] <--> Channel <--> computer.gate[1];
        computer.gate[0] <--> Channel <--> cloud.gate[1];
}
