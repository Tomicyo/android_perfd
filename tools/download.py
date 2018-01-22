import os, sys, platform, threading, time, urllib2, urllib, zipfile, tempfile, hashlib

max_thread = 10
lock = threading.RLock()

class Downloader(threading.Thread):
    def __init__(self, url):
        self.url = url
        self.threadNum = 8
        threading.Thread.__init__(self)

    def getFilename(self):
        url = self.url
        protocol, s1 = urllib.splittype(url)
        host, path = urllib.splithost(s1)
        filename = path.split('/')[-1]
        if '.' not in filename:
            filename = None
        print "Do you want to change a filename?('y' or other words)"
        answer = raw_input()
        if answer == "y" or filename is None:
            print "Please input your new filename:"
            filename = raw_input()
        return filename

    def getLength(self):
        opener = urllib2.build_opener()
        req = opener.open(self.url)
        meta = req.info()
        length = int(meta.getheaders("Content-Length")[0])
        return length

    def get_range(self):
        ranges = []
        length = self.getLength()
        print 'Url: %s\n\tLength: %d MB' % (self.url, length / 1024 / 1024)
        offset = int(int(length) / self.threadNum)
        for i in range(self.threadNum):
            if i == (self.threadNum - 1):
                ranges.append((i*offset,''))
            else:
                ranges.append((i*offset,(i+1)*offset))
        return ranges

    def downloadThread(self, start, end, thread_id):
        req = urllib2.Request(self.url)
        req.headers['Range'] = 'bytes=%s-%s' % (start, end)
        f = urllib2.urlopen(req)
        offset = start
        buffer = 1024 * 512 * 5 # 512KB Once
        while 1:
            t_start = time.time()
            block = f.read(buffer)
            t_delta = time.time() - t_start
            if t_delta!=0:
                sys.stdout.write("\r### thread %d speed %d KB/s" % (thread_id, int(len(block) / 1024 / t_delta)) )
                sys.stdout.flush()
            if not block:
                break
            with lock:
                self.file.seek(offset)
                self.file.write(block)
                offset = offset + len(block)

    def download(self, fileName=None):
        filename = fileName if fileName else self.getFilename()
        self.file = open(filename, 'wb')
        thread_list = []
        n = 1
        self.time_begin = time.time()
        for ran in self.get_range():
            start, end = ran
            print 'starting:%d thread '% n
            n += 1
            thread = threading.Thread(target=self.downloadThread,args=(start, end, n))
            thread.start()
            thread_list.append(thread)

        for i in thread_list:
            i.join()
            
        self.file.close()

def download_and_extract(url, extract_dir):
    h = hashlib.md5()
    h.update(url)
    filename = str(h.hexdigest())
    down = Downloader(url)
    dfile_path = os.path.join(tempfile.gettempdir(), filename + '.zip');
    down.download(dfile_path)
    zipFile = zipfile.ZipFile(dfile_path)
    zipFile.extractall(extract_dir)
    zipFile.close()