/* <!-- copyright */
/*
 * aria2 - a simple utility for downloading files faster
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* copyright --> */
#include "PeerMessage.h"
#include "Util.h"

void PeerMessage::setBitfield(const unsigned char* bitfield, int bitfieldLength) {
  if(this->bitfield != NULL) {
    delete [] bitfield;
  }
  this->bitfieldLength = bitfieldLength;
  this->bitfield = new unsigned char[this->bitfieldLength];
  memcpy(this->bitfield, bitfield, this->bitfieldLength);
}

void PeerMessage::setBlock(const char* block, int blockLength) {
  if(this->block != NULL) {
    delete [] block;
  }
  this->blockLength = blockLength;
  this->block = new char[this->blockLength];
  memcpy(this->block, block, this->blockLength);
}

string PeerMessage::toString() const {
  switch(id) {
  case CHOKE:
    return "choke";
  case UNCHOKE:
    return "unchoke";
  case INTERESTED:
    return "interested";
  case NOT_INTERESTED:
    return "not interested";
  case HAVE:
    return "have index="+Util::itos(index);
  case BITFIELD:
    return "bitfield "+Util::toHex(bitfield, bitfieldLength);
  case REQUEST:
    return "request index="+Util::itos(index)+", begin="+Util::itos(begin)+
      ", length="+Util::itos(length);
  case PIECE:
    return "piece index="+Util::itos(index)+", begin="+Util::itos(begin)+
      ", length="+Util::itos(blockLength);
  case CANCEL:
    return "calcel index="+Util::itos(index)+", begin="+Util::itos(begin)+
      ", length="+Util::itos(length);
  case KEEP_ALIVE:
    return "keep alive";
  default:
    return "unknown";
  }
}

