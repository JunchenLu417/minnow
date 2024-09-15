#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_( capacity_ + 1 ) { }

bool Writer::is_closed() const
{
  // Your code here.
  return this->close_;
}

void Writer::push( string data )
{
  uint64_t len = min(data.size(), available_capacity());  // total bytes pushed
  uint64_t first_chunk = min(len, buffer_.size() - write_pos);  // before the end of buffer
  copy(data.data(), data.data() + first_chunk, buffer_.data() + write_pos);
  if (len > first_chunk)
    copy(data.data() + first_chunk, data.data() + len, buffer_.data());
  write_pos = (write_pos + len) % buffer_.size();
  bytes_pushed_total += len;
}

void Writer::close()
{
  // Your code here.
  this->close_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return this->capacity_ - this->reader().bytes_buffered();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return this->bytes_pushed_total;
}

bool Reader::is_finished() const
{
  // Your code here.
  return this->close_ && this->bytes_buffered() == 0;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return this->bytes_pushed_total - this->bytes_buffered();
}

string_view Reader::peek() const
{
  // Your code here.
  if (bytes_buffered() == 0)
    return string_view();  // return empty view if no data
  else
    return string_view(this->buffer_.data() + this->read_pos, 
      min(this->bytes_buffered(), buffer_.size() - read_pos));  // peek next bytes in the buffer (chunk till the end)
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  len = min(len, bytes_buffered());
  read_pos = (read_pos + len) % buffer_.size();
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return write_pos >= read_pos ? write_pos - read_pos : buffer_.size() - read_pos + write_pos;
}
