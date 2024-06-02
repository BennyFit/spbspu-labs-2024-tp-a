#include "commands.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include "entities.hpp"
#include "huffmanAlgorithm.hpp"

namespace ibragimov
{
  namespace detail
  {
    Encodings createEncodings(const DecodedText&);
    EncodedText encode(const DecodedText&, const Encodings&);
    DecodedText decode(const EncodedText&, const Encodings&);
    template < class T >
    T getData(const std::shared_ptr< T >&);
  }
}

void ibragimov::input(const std::map< std::string, std::function< void(std::istream&) > >& subCommands, std::istream& in)
{
  std::string input{};
  in >> input;
  std::function< void(std::istream&) > command;
  try
  {
    command = subCommands.at(input);
    command(in);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("");
  }
}
void ibragimov::saveIntoMemory(const std::map< std::string, std::function< void() > >& subCommands, std::istream& in)
{
  std::string input{};
  in >> input;
  std::function< void() > command;
  try
  {
    command = subCommands.at(input);
    command();
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("");
  }
}
void ibragimov::deleteFromMemory(const std::map< std::string, std::function< void(std::istream&) > >& subCommands, std::istream& in)
{
  std::string input{};
  in >> input;
  std::function< void(std::istream&) > command;
  try
  {
    command = subCommands.at(input);
    command(in);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("");
  }
}
void ibragimov::loadFromMemory(const std::map< std::string, std::function< void(std::istream&) > >& subCommands, std::istream& in)
{
  std::string input{};
  in >> input;
  std::function< void(std::istream&) > command;
  try
  {
    command = subCommands.at(input);
    command(in);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("");
  }
}
void ibragimov::printInfo(const std::map< std::string, std::function< void() > >& subCommands, std::istream& in)
{
  std::string input{};
  in >> input;
  std::function< void() > command;
  try
  {
    command = subCommands.at(input);
    command();
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("");
  }
}
void ibragimov::huffman(const std::vector< std::shared_ptr< DecodedText > >& texts, const size_t pos, std::shared_ptr< Entity >& current)
{
  std::shared_ptr< Encodings > encoding = std::make_shared< Encodings >(detail::createEncodings(*texts.at(pos)));
  current = std::move(encoding);
}
void ibragimov::encode(const std::vector< std::shared_ptr< DecodedText > >& texts, const size_t pos1,
    const std::vector< std::shared_ptr< Encodings > >& encodings, const size_t pos2, std::shared_ptr< Entity >& current)
{
  std::shared_ptr< EncodedText > encoded = std::make_shared< EncodedText >(detail::encode(*texts.at(pos1), *encodings.at(pos2)));
  current = std::move(encoded);
}
void ibragimov::decode(const std::vector< std::shared_ptr< EncodedText > >& texts, const size_t pos1,
    const std::vector< std::shared_ptr< Encodings > >& encodings, const size_t pos2, std::shared_ptr< Entity >& current)
{
  std::shared_ptr< DecodedText > decoded = std::make_shared< DecodedText >(detail::decode(*texts.at(pos1), *encodings.at(pos2)));
  current = std::move(decoded);
}

void ibragimov::printCurrent(const std::shared_ptr< Entity >& current)
{
  if (!current)
  {
    throw std::invalid_argument("");
  }
  else
  {
    current->outputInfo();
  }
}
void ibragimov::printAll(const std::vector< std::shared_ptr< DecodedText > >& decoded,
    const std::vector< std::shared_ptr< EncodedText > >& encoded,
    const std::vector< std::shared_ptr< Encodings > >& encodings)
{
  printDecoded(decoded);
  printEncoded(encoded);
  printEncodings(encodings);
}
void ibragimov::printDecoded(const std::vector< std::shared_ptr< DecodedText > >& decoded)
{
  if (decoded.empty())
  {
    throw std::invalid_argument("");
  }
  using os_iter = std::ostream_iterator< DecodedText >;
  std::transform(decoded.cbegin(), decoded.cend(), os_iter{std::cout << "DECODED:\n", "\n"}, detail::getData< DecodedText >);
}
void ibragimov::printEncoded(const std::vector< std::shared_ptr< EncodedText > >& encoded)
{
  if (encoded.empty())
  {
    throw std::invalid_argument("");
  }
  using os_iter = std::ostream_iterator< EncodedText >;
  std::transform(encoded.cbegin(), encoded.cend(), os_iter{std::cout << "ENCODED:\n", "\n"}, detail::getData< EncodedText >);
}
void ibragimov::printEncodings(const std::vector< std::shared_ptr< Encodings > >& encodings)
{
  if (encodings.empty())
  {
    throw std::invalid_argument("");
  }
  using os_iter = std::ostream_iterator< Encodings >;
  std::transform(encodings.cbegin(), encodings.cend(), os_iter{std::cout << "ENCODINGS:\n", "\n"}, detail::getData< Encodings >);
}

size_t ibragimov::inputPos(std::istream& in)
{
  size_t pos = 0;
  in >> pos;
  if (!in)
  {
    throw std::invalid_argument("");
  }
  return pos;
}

ibragimov::Encodings ibragimov::detail::createEncodings(const DecodedText& text)
{
  return Encodings(createEncodingTable(text.text));
}
ibragimov::EncodedText ibragimov::detail::encode(const DecodedText& text, const Encodings& table)
{
  return EncodedText(encode(text.text, table.encodingTable));
}
ibragimov::DecodedText ibragimov::detail::decode(const EncodedText& text, const Encodings& table)
{
  return DecodedText(decode(text.text, table.encodingTable));
}
template < class T >
T ibragimov::detail::getData(const std::shared_ptr< T >& rhs)
{
  return *rhs;
}
