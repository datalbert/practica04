
#include "ns3/command-line.h"
#include "ns3/core-module.h"
#include "ns3/simulator.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/net-device-container.h"
#include "ns3/node.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/udp-server.h"


#include "ns3/point-to-point-channel.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Practica04"); 


int main (int argc, char *argv[])
{
    Ptr<Node> n_servidor=CreateObject<Node>();
    Nodecontainer c_todos (n_servidor);
    Ptr<Node> n_cliente=CreateObject<Node>();
    c_todos.add(n_cliente);
    

    InternetStackHelper h_pila;
    h_pila.SetIpv6StackInstall (false);
    h_pila.Install (c_todos);
     
    //Capa ppp, capa común, lo hacemos directamente desde el contenedor. 
    PointToPointHelper h_pro;
    NetDeviceContainer c_dispositivo=h_pro.install(c_todos);

    //Capa ip
    Ipv4AddressHelper direcciones("10.1.13.0","255.255.255.0");
    Ipv4InterfacesContainer c_interfaces=direcciones.Assign(c_todos);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


    //Capa de aplicación.
    UdpEchoServerHelper h_echoServer(20);
    ApplicationContainer c_serverApps = h_echoServer.Install (n_servidor);
    
    UdpEchoClientHelper h_echoClient(c_interfaces.GetAddress(1),21);
    ApplicationContainer c_clientApps = h_echoClient.Install (n_cliente);	
    
    //Añadimos el remoto al udp client
    c_clientApps.Get(0)->SetRemote(
    	c_interfaces.GetAddress(0),
    	c_serverApps.Get(0)->
    );
    	
    c_clientApps.SetRemote
    
    //le decimos al cliente cual es el servidor.
    
    
    
    
    
}
