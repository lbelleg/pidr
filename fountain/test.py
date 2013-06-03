
import random
import os
import time

class Distribution(object):

    def __init__(self, k):
        self.k = k
        tabl = [ 0 ] * (k + 1)
        tabl[1] = 1 / float(k)
        for i in xrange(2, k + 1):
            tabl[i] = 1 / float(i*(i - 1))
        for i in xrange(1, k + 1):
            tabl[i] += tabl[i-1]
        tabl[k] = 1.0
        self.tabl = tabl

    def degree(self):
        r = random.random()
        for i in xrange(len(self.tabl)):
            if self.tabl[i] > r:
                return i
        raise AssertionError('impossible!')

    def random_blocks(self):
        d = self.degree()
        return d, random.sample(xrange(self.k), d)

    def random_xor(self, blocks):
        d, indices = self.random_blocks()
        msg = 0
        for i in indices:
            msg ^= blocks[i]
        return d, indices, msg


class Encoder(object):

    def __init__(self, blocks, seed = None):
        if seed is None:
            seed = random.randint(0, 2**32 - 1)
        self.seed = (seed & 0xffffffff) << 32
        self.blocks = blocks
        self.distrib = Distribution(len(blocks))

    def encode(self):
        self.seed += 1
        random.seed(self.seed)
        degree, blocks, msg = self.distrib.random_xor(self.blocks)
        return {
            'seed' : self.seed,
            'size' : len(self.blocks),
            'xor' : msg,
            '_blocks' : blocks
        }

# decoder

class Decoder(object):

    def __init__(self):
        self.size = None
        self.blocks = None
        self.decoded = None
        self.file = None
        self.recv = 0

    def simplify(self, p):
        for b in list(p['blocks']):
            if self.decoded[b]:
                p['xor'] ^= self.file[b]
                p['blocks'].remove(b)

    def reduce_for_block(self, b, thelist):
        for packet in self.blocks[b]:
            packet['blocks'].remove(b)
            packet['xor'] ^= self.file[b]
            if len(packet['blocks']) == 1:
                thelist.append(packet)
        self.blocks[b] = None # we are done with that

    def reduction(self, p):
        thelist = [ p ]
        while len(thelist) != 0:
            q = thelist.pop()
            if len(q['blocks']) != 1:
                # print 'BOOM!'
                continue
            b = list(q['blocks'])[0]
            if self.decoded[b]:
                continue
            self.decoded[b] = True
            self.recv += 1
            self.file[b] = q['xor']
            self.reduce_for_block(b, thelist)

    def initialize(self, p):
        if self.size is None:
            self.size = p['size']
            self.blocks = [ [] for x in xrange(self.size) ]
            self.decoded = [ False ] * self.size
            self.file = [ None ] * self.size
            self.distrib = Distribution(self.size)

    def decode(self, p):
        random.seed(p['seed']) # reinitialize
        self.initialize(p)
        if self.recv == self.size:
            # print "All decoded!"
            return
        # print "decode: %s" % p
        d, bs = self.distrib.random_blocks()
        p['blocks'] = set(bs)
        return self._decode(p)

    def _decode(self, p):
        self.simplify(p)
        # print "p = %s" % p
        if len(p['blocks']) == 0:
            return
        elif len(p['blocks']) == 1:
            self.reduction(p)
        else:
            for b in p['blocks']:
                if not self.decoded[b]:
                    self.blocks[b].append(p)

    def show(self):
        for i, p in enumerate(self.blocks):
            print i, p, self.decoded[i]
        print "file = %s" % self.file

    def finished(self):
        return self.recv == self.size

def measure(f):
    start = time.time()
    v = f()
    end = time.time()
    return end - start

if __name__ == '__main__':
    
    n = 20000
    fichier = range(n)

    random.seed(time.time())

    enc = Encoder(fichier)
    dec = Decoder()

    packets = []
    def encoding():
        for i in xrange(n):
            p = enc.encode()
            packets.append(p)

    def decoding():
        for p in packets:
            dec.decode(p)

    t1 = measure(encoding)
    t2 = measure(decoding)

    print 'Encoding: %.4f' % t1
    print 'Decoding: %.4f' % t2

