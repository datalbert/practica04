
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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Practica04"); 


int main (int argc, char *argv[])
{
    Ptr<Node> n_servidor=CreateObject<Node>();
    Nodocontainer c_todos (n_servidor);
    Ptr<Node> n_cliente=CreateObject<Node>();
    c_todos.add(n_cliente);
    

    InternetStackHelper h_pila;
    h_pila.SetIpv6StackInstall (false);
    h_pila.Install (c_todos);

    PointToPointHelper h_pro;
    NetDeviceContainer c_dispositivo=h_pro.install(c_todos);

    Ipv4AddressHelper direcciones("10.1.13.0","255.255.255.0");
    Ipv4InterfacesContainer c_interfaces=direcciones.Assign(c_todos);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    

}
