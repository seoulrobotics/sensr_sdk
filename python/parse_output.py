from sensr_proto.output_pb2 import OutputMessage

def label_to_string(label):
    if label == 1:
        return 'Car'
    elif label == 2:
        return 'Pedestrian'
    elif label == 3:
        return 'Cyclist'
    return 'Misc'

def parse_output_file(filename):
    with open(filename, 'rb') as f:
        output = OutputMessage()
        output.ParseFromString(f.read())
        if output.stream:
            for obj in output.stream.objects:
                print(f'Object #{obj.id}: {label_to_string(obj.label)}')
                print(obj.bbox)

def main():
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('filename', type=str)
    args = parser.parse_args()

    parse_output_file(args.filename)


if __name__ == '__main__':
    main()