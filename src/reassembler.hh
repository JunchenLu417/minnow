#pragma once

#include "byte_stream.hh"

#include <limits>

class Reassembler
{
public:
  // Construct Reassembler to write into given ByteStream.
  explicit Reassembler( ByteStream&& output ) : output_( std::move( output ) ), 
    max_idx_seen(std::numeric_limits<uint64_t>::max()), 
    buffer_(get_capacity(), std::make_pair('\0', false)) {}

  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  void insert( uint64_t first_index, std::string data, bool is_last_substring );

  // How many bytes are stored in the Reassembler itself?
  uint64_t bytes_pending() const;

  // Access output stream reader
  Reader& reader() { return output_.reader(); }
  const Reader& reader() const { return output_.reader(); }

  // Access output stream writer, but const-only (can't write from outside)
  const Writer& writer() const { return output_.writer(); }

private:
  ByteStream output_; // the Reassembler writes to this ByteStream
  uint64_t max_idx_seen;  // set only when is_last_substring

  // bytes in the Reassembler's internal storage, with valid bit
  std::vector<std::pair<char, bool>> buffer_;
  uint64_t start_idx = 0;  // this->buffer_ is also a ring buffer

  // helper functions for important indices
  uint64_t get_first_unpopped_idx() { return this->reader().bytes_popped(); }
  uint64_t get_first_unassembled_idx() { return get_first_unpopped_idx() + reader().bytes_buffered(); }
  uint64_t get_available_capacity() { return writer().available_capacity(); }
  uint64_t get_capacity() { return reader().bytes_buffered() + get_available_capacity(); }
  uint64_t get_first_unacceptable_idx() { return get_first_unpopped_idx() + get_capacity(); }
};
