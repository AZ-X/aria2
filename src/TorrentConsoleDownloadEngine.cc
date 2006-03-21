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
#include "TorrentConsoleDownloadEngine.h"
#include "Util.h"

TorrentConsoleDownloadEngine::TorrentConsoleDownloadEngine() {}

TorrentConsoleDownloadEngine::~TorrentConsoleDownloadEngine() {}

void TorrentConsoleDownloadEngine::printStatistics() {
  printf("\r                                                                            ");
  printf("\r");
  printf("%s/%sB %d%% DW:%.2f UP:%.2f(%s) %dpeers",
	 Util::llitos(torrentMan->getDownloadedSize(), true).c_str(),
	 Util::llitos(torrentMan->totalSize, true).c_str(),
	 (torrentMan->totalSize == 0 ?
	  0 : (int)((torrentMan->getDownloadedSize()*100)/torrentMan->totalSize)),
	 downloadSpeed/1000.0,
	 uploadSpeed/1000.0,
	 Util::llitos(torrentMan->getUploadedSize(), true).c_str(),
	 torrentMan->connections);
  fflush(stdout);	 
}

void TorrentConsoleDownloadEngine::initStatistics() {
  /*
  gettimeofday(&cp, NULL);
  */
  downloadSpeed = 0;
  uploadSpeed = 0;
  /*
  sessionDownloadSize = 0;
  sessionUploadSize = 0;
  */
  lastElapsed = 0;
  gettimeofday(&cp[0], NULL);
  gettimeofday(&cp[1], NULL);
  sessionDownloadSize[0] = 0;
  sessionDownloadSize[1] = 0;
  sessionUploadSize[0] = 0;
  sessionUploadSize[1] = 0;
  currentCp = 0;
}

/*
int TorrentConsoleDownloadEngine::calculateSpeed(int deltaSize, long long int elapsed, int prevSpeed) {
  int nowSpeed = (int)(deltaSize/(elapsed/1000000.0));
  return (nowSpeed+prevSpeed)/2;
}
*/
int TorrentConsoleDownloadEngine::calculateSpeed(long long int sessionSize, long long int elapsed) {
  int nowSpeed = (int)(sessionSize/(elapsed/1000000.0));
  return nowSpeed;
}

void TorrentConsoleDownloadEngine::calculateStatistics() {
  /*
  struct timeval now;
  gettimeofday(&now, NULL);
  long long int elapsed = Util::difftv(now, cp);
  sessionDownloadSize += torrentMan->getDeltaDownload();
  sessionUploadSize += torrentMan->getDeltaUpload();
  
  downloadSpeed = calculateSpeed(sessionDownloadSize, elapsed);
  uploadSpeed = calculateSpeed(sessionUploadSize, elapsed);
  torrentMan->resetDeltaDownload();
  torrentMan->resetDeltaUpload();
  if(elapsed-lastElapsed >= 1000000) {
    printStatistics();
    lastElapsed = elapsed;
  }
  */
  struct timeval now;
  gettimeofday(&now, NULL);
  long long int elapsed = Util::difftv(now, cp[currentCp]);

  sessionDownloadSize[0] += torrentMan->getDeltaDownload();
  sessionUploadSize[0] += torrentMan->getDeltaUpload();
  sessionDownloadSize[1] += torrentMan->getDeltaDownload();
  sessionUploadSize[1] += torrentMan->getDeltaUpload();

  downloadSpeed = calculateSpeed(sessionDownloadSize[currentCp], elapsed);
  uploadSpeed = calculateSpeed(sessionUploadSize[currentCp], elapsed);

  torrentMan->resetDeltaDownload();
  torrentMan->resetDeltaUpload();

  if(elapsed-lastElapsed >= 1000000) {
    printStatistics();
    lastElapsed = elapsed;
  }

  if(elapsed > 15*1000000) {
    sessionDownloadSize[currentCp] = 0;
    sessionUploadSize[currentCp] = 0;
    cp[currentCp] = now;
    lastElapsed = 0;
    currentCp = currentCp ? 0 : 1;
  }

    /*
    if(elapsed >= 1000000) {
      downloadSpeed = calculateSpeed(torrentMan->getDeltaDownload(),
				     elapsed, downloadSpeed);
      uploadSpeed = calculateSpeed(torrentMan->getDeltaUpload(),
				   elapsed, uploadSpeed);
      torrentMan->resetDeltaDownload();
      torrentMan->resetDeltaUpload();
      cp = now;
      printStatistics();
    }
    */
}
