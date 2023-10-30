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
        @display("i=misc/cloud"); // add a default icon
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
    @figure[Library](type=rectangle; pos=200,200; size=600,350; fillColor=gray; fillOpacity=0.2);
    @figure[Library.label](type=text; pos=210,210; text=Library);

    @figure[Pillar_1](type=rectangle; pos=400,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar_1.label](type=text; pos=210,210; text=Library);
    @figure[Shelf_1](type=rectangle; pos=360,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf_1.label](type=text; pos=210,210; text=Library);


    @figure[Pillar_2](type=rectangle; pos=590,300; size=20,250; fillColor=black; fillOpacity=1);
    @figure[Pillar_2.label](type=text; pos=210,210; text=Library);
    @figure[Shelf_2](type=rectangle; pos=610,300; size=40,250; fillColor=white; fillOpacity=0.8);
    @figure[Shelf_2.label](type=text; pos=210,210; text=Library);
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        cloud: Cloud {
            @display("p=1127.808,49.280003;r=725");
        }
        host: MobileHost {
            @display("p=500,500.44;r=80");
        }
        computer: Computer {
            @display("p=500,280;r=180");
        }
        visualizer: IntegratedVisualizer {
            @display("p=50,50");
        }

    connections:
        host.gate[0] <--> Channel <--> cloud.gate[0];
        host.gate[1] <--> Channel <--> computer.gate[1];
        computer.gate[0] <--> Channel <--> cloud.gate[1];
}