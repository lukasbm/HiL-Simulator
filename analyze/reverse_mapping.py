#
# mapping measured data to omnet components
#

readin = {
    ('T0', 'dT0', 'B0'): 'T0_rosbag',
    ('T1', 'dT1', 'B1'): 'T1_replayer',
    ('T2', 'dT2', 'B2'): 'TqROS2_out',
    ('T3', 'dT3', 'B3'): 'TqROS3_in',
    ('T4', 'dT4', 'B4'): 'TqROS4_out',
    ('T5', 'dT5', 'B5'): 'Tp2_out',
    ('T6', 'dT6', 'B6'): 'TqLV0_out',
    ('T7', 'dT7', 'B7'): 'TqLV1_out',
    ('T8', 'dT8', 'B8'): 'TqLV2_in',
    ('T9', 'dT9', 'B9'): 'TqLV2_out',
    ('T10', 'dT10', 'B10'): 'Tp4_out',
}

mapping = {
    # Ros
    'TqROS1_in': ('dropTailQueue_ROS1', 'packetPushed'),
    'TqROS2_out': ('dropTailQueue_ROS2', 'packetPopped'),
    'Tp1_out': ('packetServer', 'packetServed'),
    'TqROS3_in': ('dropTailQueue_ROS3', 'packetPushed'),
    'TqROS4_out': ('dropTailQueue_ROS4', 'packetPopped'),
    'Tp2_out': ('packetServer1', 'packetServed'),
    'T0_rosbag': ('activePacketSource', 'packetCreated'),
    'T1_replayer': ('packetServerROSReplayer', 'packetServed'),
    # LabView
    'TqLV0_in': ('dropTailQueueLV0', 'packetPushed'),
    'TqLV0_out': ('dropTailQueueLV0', 'packetPopped'),
    'TqLV1_in': ('dropTailQueueLV1', 'packetPushed'),
    'TqLV1_out': ('dropTailQueueLV1', 'packetPopped'),
    'TqLV2_in': ('dropTailQueueLV2', 'packetPushed'),
    'TqLV2_out': ('dropTailQueueLV2', 'packetPopped'),
    'Tp4_out': ('Sink', 'packetDropped')
}
