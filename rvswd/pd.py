##
## This file is part of the libsigrokdecode project.
##
## Copyright (C) 2021 Tianyi Wang <163.wty@163.com>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, see <http://www.gnu.org/licenses/>.
##

# TODO: Output transaction description.
# TODO: Output bits.
# TODO: Implement support for detecting various bus errors.

import sigrokdecode as srd

# CMD: [annotation-type-index, long annotation, short annotation, bit count]
proto = {
    'START':           [0, 'Start',         'S',  0],
    'START REPEAT':    [1, 'Start repeat',  'Sr', 0],
    'STOP':            [2, 'Stop',          'P',  0],
    'ADDRESS WRITE':   [3, 'Address write', 'AW', 7],
    'DATA WRITE':      [4, 'Data write',    'DW', 32],
    'OP WRITE':        [5, 'Op write',      'OW', 2],
    'BUS TURN':        [6, 'Bus Turning',   'BT', 1],
    'ADDRESS READ':    [7, 'Address read',  'AR', 7],
    'DATA READ':       [8, 'Data read',     'DR', 32],
    'OP READ':         [9, 'Op read',       'OR', 2],
    'FIN':             [10, 'Final mark',   'FM', 1],
}

dmi_reg = {
    0x04: ['Abstract Data 0', 'data0'],
    0x05: ['Abstract Data 1', 'data1'],
    0x06: ['Abstract Data 2', 'data2'],
    0x07: ['Abstract Data 3', 'data3'],
    0x08: ['Abstract Data 4', 'data4'],
    0x09: ['Abstract Data 5', 'data5'],
    0x0a: ['Abstract Data 6', 'data6'],
    0x0b: ['Abstract Data 7', 'data7'],
    0x0c: ['Abstract Data 8', 'data8'],
    0x0d: ['Abstract Data 9', 'data9'],
    0x0e: ['Abstract Data 10', 'data10'],
    0x0f: ['Abstract Data 11', 'data11'],
    0x10: ['Debug Module Control', 'dmcontrol'],
    0x11: ['Debug Module Status', 'dmstatus'],
    0x12: ['Hart Info', 'hartinfo'],
    0x13: ['Halt Summary 1', 'haltsum1'],
    0x14: ['Hart Array Window Select', 'hawindowsel'],
    0x15: ['Hart Array Window', 'hawindow'],
    0x16: ['Abstract Control and Status', 'abstractcs'],
    0x17: ['Abstract Command', 'command'],
    0x18: ['Abstract Command Autoexec', 'abstractauto'],
    0x19: ['Configuration String Pointer 0', 'confstrptr0'],
    0x1a: ['Configuration String Pointer 1', 'confstrptr1'],
    0x1b: ['Configuration String Pointer 2', 'confstrptr2'],
    0x1c: ['Configuration String Pointer 3', 'confstrptr3'],
    0x1d: ['Next Debug Module', 'nextdm'],
    0x1f: ['Custom Features', 'custom'],
    0x20: ['Program Buffer 0', 'progbuf0'],
    0x21: ['Program Buffer 1', 'progbuf1'],
    0x22: ['Program Buffer 2', 'progbuf2'],
    0x23: ['Program Buffer 3', 'progbuf3'],
    0x24: ['Program Buffer 4', 'progbuf4'],
    0x25: ['Program Buffer 5', 'progbuf5'],
    0x26: ['Program Buffer 6', 'progbuf6'],
    0x27: ['Program Buffer 7', 'progbuf7'],
    0x28: ['Program Buffer 8', 'progbuf8'],
    0x29: ['Program Buffer 9', 'progbuf9'],
    0x2a: ['Program Buffer 10', 'progbuf10'],
    0x2b: ['Program Buffer 11', 'progbuf11'],
    0x2c: ['Program Buffer 12', 'progbuf12'],
    0x2d: ['Program Buffer 13', 'progbuf13'],
    0x2e: ['Program Buffer 14', 'progbuf14'],
    0x2f: ['Program Buffer 15', 'progbuf15'],
    0x30: ['Authentication Data', 'authdata'],
    0x32: ['Debug Module Control and Status 2', 'dmcs2'],
    0x34: ['Halt Summary 2', 'haltsum2'],
    0x35: ['Halt Summary 3', 'haltsum3'],
    0x37: ['System Bus Address 127:96', 'sbaddress3'],
    0x38: ['System Bus Access Control and Status', 'sbcs'],
    0x39: ['System Bus Address 31:0', 'sbaddress0'],
    0x3a: ['System Bus Address 63:32', 'sbaddress1'],
    0x3b: ['System Bus Address 95:64', 'sbaddress2'],
    0x3c: ['System Bus Data 31:0', 'sbdata0'],
    0x3d: ['System Bus Data 63:32', 'sbdata1'],
    0x3e: ['System Bus Data 95:64', 'sbdata2'],
    0x3f: ['System Bus Data 127:96', 'sbdata3'],
    0x40: ['Halt Summary 0', 'haltsum0'],
}

dmi_req = [
    ['NOP',      'N'],
    ['READ',     'R'],
    ['WRITE',    'W'],
    ['RESERVED', 'E'],
];

dmi_resp = [
    ['SUCCESS',  'S'],
    ['RESERVED', 'E'],
    ['FAIL',     'F'],
    ['BUSY',     'B'],
];

class Decoder(srd.Decoder):
    api_version = 3
    id = 'rvswd'
    name = 'RVSWD'
    longname = 'RISCV Serial Wire Debug'
    desc = 'Two-wire, serial debug for WCH RISCV MCUS.'
    license = 'gplv2+'
    inputs = ['logic']
    outputs = ['rvswd']
    tags = ['Debug/trace']
    channels = (
        {'id': 'swclk', 'type': 8, 'name': 'SWCLK', 'desc': 'Serial clock line'},
        {'id': 'swdio', 'type': 108, 'name': 'SWDIO', 'desc': 'Serial data line'},
    )
    options = ()
    annotations = (
        ('7', 'start', 'Start condition'),
        ('6', 'repeat-start', 'Repeat start condition'),
        ('1', 'stop', 'Stop condition'),
        ('5', 'op-read', 'Op Read'),
        ('0', 'op-write', 'Op Write'),
        ('112', 'address-read', 'Address read'),
        ('111', 'address-write', 'Address write'),
        ('110', 'data-read', 'Data read'),
        ('109', 'data-write', 'Data write'),
        ('113', 'fin', 'Final Mark'),
        ('114', 'bus-turn', 'Bus turning'),
        ('1000', 'description', 'Description of the sequence'),
    )
    annotation_rows = (
        ('addr-data', 'Address/Data', (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
        ('description', 'Description', (11,)),
    )

    def __init__(self):
        self.reset()

    def reset(self):
        self.samplerate = None
        self.ss = self.es = self.ss_byte = -1
        self.bitcount = 0
        self.databyte = 0
        self.is_repeat_start = 0
        self.state = 'START'
        self.pdu_start = None
        self.pdu_bits = 0
        self.bits = []

    def metadata(self, key, value):
        if key == srd.SRD_CONF_SAMPLERATE:
            self.samplerate = value

    def start(self):
        self.out_ann = self.register(srd.OUTPUT_ANN)

    def putx(self, data):
        self.put(self.ss, self.es, self.out_ann, data)

    def handle_start(self):
        self.ss, self.es = self.samplenum, self.samplenum
        self.pdu_start = self.samplenum
        self.pdu_bits = 0
        cmd = 'START REPEAT' if (self.is_repeat_start == 1) else 'START'
        self.putx([proto[cmd][0], proto[cmd][1:2]])
        self.state = 'ADDRESS WRITE'
        self.bitcount = self.databyte = 0
        self.is_repeat_start = 1
        self.bits = []

    def handle_data(self, swclk, swdio):
        self.pdu_bits += 1

        # Address and data are transmitted MSB-first.
        self.databyte <<= 1
        self.databyte |= swdio
        self.bitcount += 1

        # Remember the start of the first data/address bit.
        if self.bitcount == 1:
            self.ss_byte = self.samplenum

        # Store individual bits and their start/end samplenumbers.
        # In the list, index 0 represents the LSB (I²C transmits MSB-first).
        self.bits.insert(0, [swdio, self.samplenum, self.samplenum])
        if self.bitcount > 1:
            self.bits[1][2] = self.samplenum
            self.bitwidth = self.bits[1][2] - self.bits[1][1]
        if self.bitcount == proto[self.state][3]:
            self.bits[0][2] += self.bitwidth

        # Return if we haven't collected all proto[self.state][3] bits, yet.
        if self.bitcount < proto[self.state][3]:
            return

        d = self.databyte

        self.ss, self.es = self.ss_byte, self.samplenum

        cmd = self.state
        if self.state == 'OP WRITE':
            self.putx([proto[cmd][0], dmi_req[d]])
        elif self.state == 'OP READ':
            self.putx([proto[cmd][0], dmi_resp[d]])
        elif self.state == 'ADDRESS WRITE' or self.state == 'ADDRESS READ':
            if dmi_reg.__contains__(d):
                self.putx([proto[cmd][0], dmi_reg[d]])
            else:
                cmd = self.state
                self.putx([proto[cmd][0], ['%s: %01X' % (proto[cmd][1], d),
                           '%s: %01X' % (proto[cmd][2], d), '%01X' % d]])
        else:
            if proto[self.state][3] <= 4:
                fmt = '%01X'
            elif proto[self.state][3] <= 8:
                fmt = '%02X'
            elif proto[self.state][3] <= 16:
                fmt = '%04X'
            elif proto[self.state][3] <= 32:
                fmt = '%08X'
            else:
                fmt = '%X'
            self.putx([proto[cmd][0], [('%s: ' + fmt) % (proto[cmd][1], d),
                       ('%s: ' + fmt) % (proto[cmd][2], d), fmt % d]])

        # Done with this packet.
        self.bitcount = self.databyte = 0
        self.bits = []
        if self.state == 'ADDRESS WRITE':
            self.state = 'DATA WRITE'
        elif self.state == 'DATA WRITE':
            self.state = 'OP WRITE'
        elif self.state == 'OP WRITE':
            self.state = 'BUS TURN'
        elif self.state == 'BUS TURN':
            self.state = 'ADDRESS READ'
        elif self.state == 'ADDRESS READ':
            self.state = 'DATA READ'
        elif self.state == 'DATA READ':
            self.state = 'OP READ'
        elif self.state == 'OP READ':
            self.state = 'FIN'
        elif self.state == 'FIN':
            self.state = 'ADDRESS WRITE'
        else:
            self.state = 'START'

    def handle_stop(self):
        cmd = 'STOP'
        self.ss, self.es = self.samplenum, self.samplenum
        self.putx([proto[cmd][0], proto[cmd][1:2]])
        self.state = 'START'
        self.is_repeat_start = 0
        self.bits = []

    def decode(self):
        while True:
            # State machine.
            if self.state == 'START':
                # Wait for a START condition (S): SWCLK = high, SWDIO = falling.
                self.wait({0: 'h', 1: 'f'})
                self.handle_start()
            else:
                # Wait for any of the following conditions (or combinations):
                #  a) Data sampling of receiver: SWCLK = rising, and/or
                #  b) START condition (S): SWCLK = high, SWDIO = falling, and/or
                #  c) STOP condition (P): SWCLK = high, SWDIO = rising
                (swclk, swdio) = self.wait([{0: 'r'}, {0: 'h', 1: 'f'}, {0: 'h', 1: 'r'}])

                # Check which of the condition(s) matched and handle them.
                if (self.matched & (0b1 << 0)):
                    self.handle_data(swclk, swdio)
                elif (self.matched & (0b1 << 1)):
                    pass
                    # there seems be a glitch in WCH's WCH-Link when transmitting the FIN bit
                    # the glitch looks like a Start, but I think it's not.
                    #self.handle_start()
                elif (self.matched & (0b1 << 2)):
                    self.handle_stop()

