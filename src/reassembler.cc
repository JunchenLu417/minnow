#include "reassembler.hh"

#include <algorithm>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  uint64_t substr_start = max(first_index, get_first_unassembled_idx());
  uint64_t substr_end = min(first_index + data.size(), get_first_unacceptable_idx());
  if (substr_end > substr_start) {
    data = data.substr(substr_start - first_index, substr_end - substr_start);
    // write data substr into the reassembler buffer
    substr_start = start_idx + substr_start - get_first_unassembled_idx();
    substr_end = start_idx + substr_end - get_first_unassembled_idx();
    uint64_t first_chunk_len = max(buffer_.size() - substr_start, (uint64_t) 0);
    uint64_t second_chunk_len = substr_end - substr_start - first_chunk_len;
    auto mark = [] (char c) { return make_pair(c, true); };
    if (first_chunk_len)
      transform(data.begin(), data.begin() + first_chunk_len, buffer_.begin() + substr_start, mark);
    if (second_chunk_len)
      transform(data.end() - second_chunk_len, data.end(), buffer_.begin(), mark);
    // write buffer to ByteStream when available
    if ((buffer_.begin() + start_idx)->second) {  // continue till a false position
      string to_write = "";
      for (uint64_t i = 0; i < buffer_.size(); ++i) {
        if ((buffer_.begin() + start_idx)->second) {
          to_write += (buffer_.begin() + start_idx)->first;
          (buffer_.begin() + start_idx)->second = false;
          start_idx = (start_idx + 1) % buffer_.size();
        } else {
          break;
        }
      }
      this->output_.writer().push(to_write);
    }
  }
  // close the stream after writing the last byte
  if (is_last_substring)
    this->output_.writer().close();
}

uint64_t Reassembler::bytes_pending() const
{
  return count_if(buffer_.begin(), buffer_.end(), [] (const pair<char, bool>& p) {
    return p.second;
  });
}
