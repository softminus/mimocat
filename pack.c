/* functions that handle conversion between our structures and data on the
   network */


#include "state.h"

/* converts a chunk_hdr_t into a blob of data fit for sending over the
   network. this functions assumes that dest->data is an already-allocated
   buffer of memory with length exactly CHUNK_HDR_LEN
 */
void pack_header(chunk_hdr_t *from, packed_chunk_t *dest)
{
    uint32_t beginprime, endprime, seqprime;
    /* things get converted to network byte order */
    beginprime = htonl(from->begin_off);
    endprime = htonl(from->end_off);
    seqprime = htonl(from->seq);
    
    memcpy((dest->data) + 0, &(from->index), 1);
    memcpy((dest->data) + 1, &(beginprime),  4);
    memcpy((dest->data) + 5, &(endprime),    4);
    memcpy((dest->data) + 9, &(seqprime),    4);

    dest->len = CHUNK_HDR_LEN;
}


/* converts a blob of data that is CHUNK_HDR_LEN long into a unpacked_chunk_t
   structure */

void unpack_header(uint8_t *data, size_t len, unpacked_chunk_t *dest)
{
    uint32_t begin, end, seq;
    uint8_t  index;

    assert (len == CHUNK_HDR_LEN);

    memcpy(&index, data + 0, 1);
    memcpy(&begin, data + 1, 4);
    memcpy(&end,   data + 5, 4);
    memcpy(&seq,   data + 9, 4);

    /* network byte order to host byte order */

    begin = ntohl(begin);
    end   = ntohl(end);
    seq   = ntohl(seq);

    dest->info.index = index;
    dest->info.begin_off = begin;
    dest->info.end_off = end;
    dest->info.seq = seq;
}


int main() {  
    chunk_hdr_t *from = malloc(sizeof(chunk_hdr_t));
    packed_chunk_t *to= malloc(sizeof(packed_chunk_t));
    unpacked_chunk_t *check= malloc(sizeof(unpacked_chunk_t));

    from->index     = 69;
    from->begin_off = 0xdeadbeef;
    from->end_off   = 0xdeadcafe;
    from->seq       = 0xdeaddead;

    to->data = malloc(CHUNK_HDR_LEN);

    pack_header(from, to);

    unpack_header(to->data, to->len, check);

    assert(check->info.index     == from->index );
    assert(check->info.begin_off == from->begin_off );
    assert(check->info.end_off   == from->end_off );
    assert(check->info.seq       == from->seq);
    
    free(from);
    free(to);
    free(check);
    return 0;
}




