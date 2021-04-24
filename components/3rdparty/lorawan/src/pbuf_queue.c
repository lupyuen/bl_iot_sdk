//  Packet Buffer Queue. Based on mqueue (Mbuf Queue) from Mynewt OS:
//  https://github.com/apache/mynewt-core/blob/master/kernel/os/src/os_mbuf.c
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "lwip/init.h"   //  For Lightweight IP Stack Init
#include "lwip/pbuf.h"   //  For Lightweight IP Stack pbuf 
#include "nimble_npl.h"  //  For NimBLE Porting Layer (multitasking functions)
#include "node/pbuf_queue.h"

//  TODO: Implement with NimBLE Mutex
#warning Implement critical section
#define OS_ENTER_CRITICAL(x) 
#define OS_EXIT_CRITICAL(x) 

///////////////////////////////////////////////////////////////////////////////
//  pbuf Functions

/* Allocate a pbuf for LoRaWAN transmission. This returns a pbuf with pbuf_list header, 
   LoRaWAN header and LoRaWAN payload */
struct pbuf *
alloc_pbuf(
    uint16_t header_len,   //  Header length of packet (LoRaWAN Header only, excluding pbuf_list header)
    uint16_t payload_len)  //  Payload length of packet, excluding header
{
    //  Init LWIP Buffer Pool
    static bool lwip_started = false;
    if (!lwip_started) {
        lwip_started = true;
        lwip_init();
    }
    
    //  Allocate a pbuf Packet Buffer with sufficient header space for pbuf_list header and LoRaWAN header
    struct pbuf *buf = pbuf_alloc(
        PBUF_TRANSPORT,   //  Buffer will include 182-byte transport header
        payload_len,      //  Payload size
        PBUF_RAM          //  Allocate as a single block of RAM
    );                    //  TODO: Switch to pooled memory (PBUF_POOL), which is more efficient
    assert(buf != NULL);

    //  Erase packet
    memset(buf->payload, 0, payload_len);

    //  Packet Header will contain two structs: pbuf_list Header, followed by LoRaWAN Header
    size_t combined_header_len = sizeof(struct pbuf_list) + header_len;

    //  Get pointer to pbuf_list Header and LoRaWAN Header
    void *combined_header = get_pbuf_header(buf, combined_header_len);
    void *header = get_pbuf_header(buf, header_len);

    //  Verify integrity of headers: pbuf_list Header is followed by LoRaWAN Header and LoRaWAN Payload
    assert((uint32_t) combined_header + combined_header_len == (uint32_t) buf->payload);
    assert((uint32_t) combined_header + sizeof(struct pbuf_list) == (uint32_t) header);
    assert((uint32_t) header + header_len == (uint32_t) buf->payload);

    //  Erase pbuf_list Header and LoRaWAN Header
    memset(combined_header, 0, combined_header_len);

    //  Init pbuf_list header at the start of the combined header
    struct pbuf_list *list = combined_header;
    list->header_len  = header_len;
    list->payload_len = payload_len;
    list->header      = header;
    list->payload     = buf->payload;
    list->pb          = buf;

    //  Verify integrity of pbuf_list: pbuf_list Header is followed by LoRaWAN Header and LoRaWAN Payload
    assert((uint32_t) list + sizeof(struct pbuf_list) + list->header_len == (uint32_t) list->payload);
    assert((uint32_t) list + sizeof(struct pbuf_list) == (uint32_t) list->header);
    assert((uint32_t) list->header + list->header_len == (uint32_t) list->payload);

    return buf;
}

/// Return the pbuf Packet Buffer header
void *get_pbuf_header(
    struct pbuf *buf,    //  pbuf Packet Buffer
    size_t header_size)  //  Size of header
{
    assert(buf != NULL);

    //  Shift the pbuf payload pointer BACKWARD
    //  to locate the header.
    u8_t rc = pbuf_add_header(buf, header_size);
    assert(rc == 0);

    //  Payload now points to the header
    void *header = buf->payload;
    assert(header != NULL);

    //  Shift the pbuf payload pointer FORWARD
    //  to locate the payload.
    rc = pbuf_remove_header(buf, header_size);
    assert(rc == 0);
    return header;
}

///////////////////////////////////////////////////////////////////////////////
//  pbuf Queue Functions

/**
 * Initializes a pbuf_queue.  A pbuf_queue is a queue of pbufs that ties to a
 * particular task's event queue.  pbuf_queues form a helper API around a common
 * paradigm: wait on an event queue until at least one packet is available,
 * then process a queue of packets.
 *
 * When pbufs are available on the queue, an event OS_EVENT_T_MQUEUE_DATA
 * will be posted to the task's pbuf queue.
 *
 * @param mq                    The pbuf_queue to initialize
 * @param ev_cb                 The callback to associate with the pbuf_queue
 *                              event.  Typically, this callback pulls each
 *                              packet off the pbuf_queue and processes them.
 * @param arg                   The argument to associate with the pbuf_queue event.
 *
 * @return                      0 on success, non-zero on failure.
 */
int
pbuf_queue_init(struct pbuf_queue *mq, ble_npl_event_fn *ev_cb, void *arg)
{
    struct ble_npl_event *ev;

    STAILQ_INIT(&mq->mq_head);

    ev = &mq->mq_ev;
    memset(ev, 0, sizeof(*ev));
    ev->fn = ev_cb;
    ev->arg = arg;

    return (0);
}

/**
 * Remove and return a single pbuf from the pbuf queue.  Does not block.
 *
 * @param mq The pbuf queue to pull an element off of.
 *
 * @return The next pbuf in the queue, or NULL if queue has no pbufs.
 */
struct pbuf *
pbuf_queue_get(struct pbuf_queue *mq)
{
    struct pbuf_list *mp;
    struct pbuf *m;
    //  os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    mp = STAILQ_FIRST(&mq->mq_head);
    if (mp) {
        STAILQ_REMOVE_HEAD(&mq->mq_head, next);
    }
    OS_EXIT_CRITICAL(sr);

    if (mp) {
        //  Return the pbuf referenced by the pbuf_list
        m = mp->pb;  //  Previously: OS_MBUF_PKTHDR_TO_MBUF(mp);
    } else {
        m = NULL;
    }

    return (m);
}

/**
 * Adds a packet (i.e. packet header pbuf) to a pbuf_queue. The event associated
 * with the pbuf_queue gets posted to the specified eventq.
 *
 * @param mq                    The pbuf queue to append the pbuf to.
 * @param evq                   The event queue to post an event to.
 * @param m                     The pbuf to append to the pbuf queue.
 *
 * @return 0 on success, non-zero on failure.
 */
int
pbuf_queue_put(struct pbuf_queue *mq, struct ble_npl_eventq *evq, struct pbuf *m)
{
    struct pbuf_list *mp;
    //  os_sr_t sr;

#ifdef NOTUSED
    /* Can only place the head of a chained pbuf on the queue. */
    if (!OS_MBUF_IS_PKTHDR(m)) {
        rc = OS_EINVAL;
        goto err;
    }
#endif  //  NOTUSED

    #warning get_pbuf_list
    struct pbuf_list *get_pbuf_list(struct pbuf *m);  //  TODO

    mp = get_pbuf_list(m);

    OS_ENTER_CRITICAL(sr);
    STAILQ_INSERT_TAIL(&mq->mq_head, mp, next);
    OS_EXIT_CRITICAL(sr);

    /* Only post an event to the queue if its specified */
    if (evq) {
        ble_npl_eventq_put(evq, &mq->mq_ev);
    }

    return (0);
}
