
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
#include "ns3/udp-client-server-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/data-rate.h"
#include "ns3/channel.h"
#include "ns3/nstime.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Practica04");

int main(int argc, char *argv[])
{
    //Configuración de la simulación
    //-----------------------------------------------------------
    // Resolución del simulador:
    Time::SetResolution(Time::NS);
    //----------------------------------------------------
    // Parametros y valores por defecto:
    Time duracion_simulacion = Time("10ms");
    uint32_t max_paq_cliente=0;
    Time intervalo=Time("50ns")
    uint32_t tam_paq=100;
    UintegerValue max_paq_cliente;
    TimeValue delay_channel=Time("");
    //----------------------------------------------------
    CommandLine cmd();

    cmd.AddValue("duracion_simulacion", "Duracion de la simulacion", duracion_simulacion);
    cmd.AddValue("maximoPaquetes del cliente udp", max_paq_clientes);



    cmd.Parse(argc, argv);

    //Fin configuración.
    //------------------------------------------------------------

    // Creamos nodos y los añadimos a un contenedor
    Ptr<Node> n_servidor = CreateObject<Node>();
    NodeContainer c_todos(n_servidor);
    Ptr<Node> n_cliente = CreateObject<Node>();
    c_todos.Add(n_cliente);

    InternetStackHelper h_pila;
    h_pila.SetIpv6StackInstall(false);
    h_pila.Install(c_todos);

    // Capa ppp --> Instalamos ppp a todos los nodos
    PointToPointHelper h_pro;
    NetDeviceContainer c_dispositivos = h_pro.Install(c_todos);

    // Capa ip -->
    Ipv4AddressHelper h_direcciones("10.1.13.0", "255.255.255.0");
    Ipv4InterfaceContainer c_interfaces = h_direcciones.Assign(c_dispositivos);
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Capa de aplicación.

    //-->UDP
    UdpServerHelper h_Server(20);
    ApplicationContainer c_serverApps = h_Server.Install(n_servidor);

    UdpClientHelper h_Client(c_interfaces.GetAddress(1), 21);
    ApplicationContainer c_clientApps = h_Client.Install(n_cliente);

    // Añadimos el remoto al udp client
    UintegerValue port_servidor;
    c_serverApps.Get(0)->GetObject<UdpServer>()->GetAttribute("Port",port_servidor);

    c_clientApps.Get(0)->GetObject<UdpClient>()->SetRemote(
        c_interfaces.GetAddress(0),
        port_servidor.Get()
    );

    DataRateValue v_serv;
    c_dispositivos.Get(0)->GetAttribute("DataRate",v_serv);
    DataRateValue v_client;
    c_dispositivos.Get(1)->GetAttribute("DataRate",v_client);

    
    c_dispositivos.Get(0)->GetChannel()->GetAttribute("Delay",delay_channel);

    UintegerValue max_paq_cliente;
    c_clientApps.Get(0)->GetObject<UdpClient>()->GetAttribute("MaxPackets",max_paq_cliente);
    
    TimeValue intervalo;
    c_clientApps.Get(0)->GetObject<UdpClient>()->GetAttribute("Interval",intervalo);
    
    //Sumario de nodos y ispositivos.
    //UintegerValue id_nodoServidor;
    //c_todos.Get(0)->GetAttribute("Id",id_nodoServidor);
    NS_LOG_INFO("ID nodo servidor:" << c_todos.Get(0)->GetId());
    NS_LOG_INFO("ID nodo cliente:" << c_todos.Get(1)->GetId());
    NS_LOG_INFO("Dispositivo de red Servidor:\n"
        << "Dirección Mac: " << c_dispositivos.Get(0)->GetAddress() <<", Velocidad de Tx: "<<v_serv.Get().GetBitRate());
    NS_LOG_INFO("Dispositivo de red Cliente:\n"
        << "Dirección Mac: " << c_dispositivos.Get(1)->GetAddress() <<", Velocidad de Tx: "<<v_client.Get().GetBitRate());
    NS_LOG_INFO("Retardo del canal "<< delay_channel.Get());
    NS_LOG_INFO("Puerto de escucha del servidor "<< port_servidor.Get());
    NS_LOG_INFO("Nº maximo de paquetes que el cliente puede enviar: "<< max_paq_cliente.Get());
    NS_LOG_INFO("Intervalo de envio: "<< intervalo.Get());

    
    //---------------------------------------------------------------
    //Lanzamos simulación
    NS_LOG_INFO("Arranca simulación");
    Simulator::Stop(duracion_simulacion);
    Simulator::Run();
    NS_LOG_INFO("Fin simulación");
}
