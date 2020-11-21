# cmpt471_project
refer to RFC 1034, 1035, 1536


RR(resource records) has the following:

owner           which is the domain name where the RR is found.

type            which is an encoded 16 bit value that specifies the type
                of the resource in this resource record.  Types refer to
                abstract resources.

                This memo uses the following types:

                A               a host address

                CNAME           identifies the canonical name of an
                                alias

                HINFO           identifies the CPU and OS used by a host

                MX              identifies a mail exchange for the
                                domain.  See [RFC-974 for details.

                NS
                the authoritative name server for the domain

                PTR
                a pointer to another part of the domain name space

                SOA
                identifies the start of a zone of authority]

class           which is an encoded 16 bit value which identifies a
                protocol family or instance of a protocol.

                This memo uses the following classes:

                IN              the Internet system

                CH              the Chaos system

TTL             which is the time to live of the RR.  This field is a 32
                bit integer in units of seconds, an is primarily used by
                resolvers when they cache RRs.  The TTL describes how
                long a RR can be cached before it should be discarded.




Mockapetris                                                    [Page 12]

 
RFC 1034             Domain Concepts and Facilities        November 1987


RDATA           which is the type and sometimes class dependent data
                which describes the resource:

                A               For the IN class, a 32 bit IP address

                                For the CH class, a domain name followed
                                by a 16 bit octal Chaos address.

                CNAME           a domain name.

                MX              a 16 bit preference value (lower is
                                better) followed by a host name willing
                                to act as a mail exchange for the owner
                                domain.

                NS              a host name.

                PTR             a domain name.

                SOA             several fields.



DNS query message format:

The most important field in the header is a four bit field called an
opcode which separates different queries.  Of the possible 16 values,
one (standard query) is part of the official protocol, two (inverse
query and status query) are options, one (completion) is obsolete, and
the rest are unassigned.

The four sections are:

Question        Carries the query name and other query parameters.

Answer          Carries RRs which directly answer the query.

Authority       Carries RRs which describe other authoritative servers.
                May optionally carry the SOA RR for the authoritative
                data in the answer section.

Additional      Carries RRs which may be helpful in using the RRs in the
                other sections.