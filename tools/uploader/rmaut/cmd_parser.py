import optparse

def parserFunc(unparsed_args):
    parser = optparse.OptionParser(
        usage="%prog [options]",
        description="upload tools"
    )

    group = optparse.OptionGroup(parser, "Destination")
    group.add_option("-p", "--port",
                     dest="port",
                     help="download port com",
                     default="COM10"
                     )
    group.add_option("-f", "--file",
                     dest="fw_path",
                     help="Image fw_path.",
                     metavar="FILE",
                     default=None
                     )
    group.add_option("-r", "--reset_to_bl",
                     dest="reset",
                    #  type="int",
                     help="Reset the board before upgrading",
                    #  action="store_true",
                     default=True
                     )
    group.add_option("-a", "--dst_addr",
                     dest="dst_addr",
                     help="dst_addr.",
                     default=None
                     )
    group.add_option("-e", "--erase_bytes",
                     dest="erase_bytes",
                     help="erase_bytes.",
                     type="int",
                     default=0
                     )
    group.add_option("-b", "--baud",
                     dest="baud",
                     type="int",
                     help="Serial port baud rate used when flashing/reading",
                     default=0
                     )
    parser.add_option_group(group)

    group = optparse.OptionGroup(parser, "Debug")
    group.add_option("-d", "--debug",
                     dest="debug",
                     help="Show debug output. And override loglevel with debug.",
                     action="store_true",
                     default=False
                     )
    parser.add_option_group(group)

    (options, args) = parser.parse_args(unparsed_args)

    return options
