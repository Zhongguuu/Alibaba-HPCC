import os

def post_register_types(root_module):
    enabled_features = os.environ['NS3_ENABLED_FEATURES'].split(',')

    if 'EmuNetDevice' not in enabled_features:
        for clsname in ['EmuNetDevice', 'EmuHelper']:
            root_module.classes.remove(root_module['ns3::%s' % clsname])
        root_module.enums.remove(root_module['ns3::EmuNetDevice::EncapsulationMode'])

