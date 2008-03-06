// -*- mode: c++; c-basic-offset: 4 -*-
#ifndef CLICK_TOHOST_HH
#define CLICK_TOHOST_HH
#include "elements/linuxmodule/anydevice.hh"

/*
 * =c
 *
 * ToHost([DEVNAME, I<keywords> SNIFFERS, QUIET, ALLOW_NONEXISTENT])
 *
 * =s comm
 *
 * sends packets to Linux
 *
 * =d
 *
 * Hands packets to the ordinary Linux protocol stack.
 * Expects packets with Ethernet headers.
 * 
 * You should probably give Linux IP packets addressed to
 * the local machine (including broadcasts), and a copy
 * of each ARP reply.
 *
 * If DEVNAME is present, each packet is marked to appear as if it originated
 * from that network device -- that is, its device annotation is set to that
 * device. As with ToDevice, DEVNAME can be an Ethernet address.
 *
 * This element is only available in the Linux kernel module.
 *
 * Keyword arguments are:
 *
 * =over 8
 *
 * =item SNIFFERS
 *
 * Boolean. If true, then ToHost will send packets to the kernel so that only
 * tcpdump(1), and other sniffer programs on the host, will receive them.
 * Default is false.
 *
 * =item QUIET
 *
 * Boolean.  If true, then suppress device up/down messages.  Default is false.
 *
 * =item ALLOW_NONEXISTENT
 *
 * Allow nonexistent devices. If true, and no device named DEVNAME exists when
 * the router is initialized, then ToHost will report a warning (rather than
 * an error). Later, while the router is running, if a device named DEVNAME
 * appears, ToHost will seamlessly begin using it. Default is false.
 *
 * =item UP_CALL
 *
 * Write handler.  If supplied, this handler is called when the device or link
 * comes up.
 *
 * =item DOWN_CALL
 *
 * Write handler.  If supplied, this handler is called when the device or link
 * goes down.
 *
 * =back
 *
 * =n
 *
 * Linux expects packets to have valid device annotations and packet type
 * annotations. ToHost will not pass packets with null device annotations to
 * Linux! Use the `C<ToHost(eth0)>' syntax to supply a device annotation. Most
 * packets generated by Click will have null device annotations -- for
 * example, InfiniteSource makes packets with null device annotations.
 * Exceptions include FromDevice and PollDevice.
 *
 * Linux depends on packet type annotations as well. It will generally only
 * process packets with packet type annotation HOST. (Other packets, such as
 * packets originally sent to some other host, are sent only to packet
 * sniffers like tcpdump(1). Linux will handle some BROADCAST and MULTICAST
 * packets.) By default, packets made by Click have HOST packet type
 * annotations. However, if you modified a packet that you originally got from
 * some device, that packet may have some other type. Use SetPacketType to
 * reset the type appropriately.
 *
 * Finally, IPv4 packets should have a destination IP address corresponding
 * to DEVNAME, and a routable source address. Otherwise Linux will silently
 * drop the packets.
 *
 * =h drops read-only
 *
 * Reports the number of packets ToHost has dropped because they had a null
 * device annotation.
 *
 * =a
 *
 * ToHostSniffers, FromHost, FromDevice, PollDevice, ToDevice,
 * SetPacketType, InfiniteSource */

class ToHost : public AnyDevice { public:
  
    ToHost();
    ~ToHost();

    static void static_initialize();
    static void static_cleanup();

    const char *class_name() const	{ return "ToHost"; }
    const char *port_count() const	{ return PORTS_1_0; }
    const char *processing() const	{ return PUSH; }
    const char *flags() const		{ return "S2"; }

    int configure_phase() const		{ return CONFIGURE_PHASE_TODEVICE; }
    int configure(Vector<String> &, ErrorHandler *);
    int initialize(ErrorHandler *);
    void cleanup(CleanupStage);
    void add_handlers();

    void push(int port, Packet *);

  private:

    bool _sniffers;
    int _drops;

    static String read_handler(Element *, void *);

    friend class ToHostSniffers;
    
};

#endif
