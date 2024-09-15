#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), bytes_pushed_total(0), buffer("") {
  // Reserve the buffer capacity upfront to minimize reallocations.
  this->buffer.reserve(2 * this->capacity_);
}

bool Writer::is_closed() const
{
  // Your code here.
  return this->close_;
}

void Writer::push( string data )
{
  // Your code here.
  // (void)data;
  if (data.size() <= this->available_capacity()) {
    this->buffer += data;  // append directly
    this->bytes_pushed_total += data.size();
  } else {
    this->bytes_pushed_total += this->available_capacity();
    this->buffer += data.substr(0, this->available_capacity());
  }
  return;
}

void Writer::close()
{
  // Your code here.
  this->close_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return this->capacity_ - this->buffer.size() + this->read_pos;
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
  if (this->read_pos >= this->buffer.size())
    return string_view();  // return empty view if no data
  else
    return string_view(this->buffer.data() + this->read_pos, 1);  // peek next byte
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  // (void)len;
  if (this->read_pos + len >= this->buffer.size()) {  // empty the buffer
    this->buffer.clear();
    this->read_pos = 0;
  } else {
    this->read_pos += len;
    if (this->read_pos >= this->buffer.size() / 2) {
      this->buffer.erase(0, this->read_pos);  // otherwise the buffer is ever-growing
      this->read_pos = 0;
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return this->buffer.size() - this->read_pos;
}
