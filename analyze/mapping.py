#
# mapping omnet export data (statistics) to measured data (CAN.csv)
#
# timestamp ist T
# value ist B (byte)
#
# latencies selber berechnen T1 - T0
# diff T1 = dt1 (diff funktion zwischen zwei messungen)
#

readin = {
    # Ros
    ('dropTailQueue_ROS1', 'packetPushed'): 'TqROS1_in',
    ('dropTailQueue_ROS2', 'packetPopped'): 'TqROS2_out',
    ('packetServer', 'packetServed'): 'Tp1_out',
    ('dropTailQueue_ROS3', 'packetPushed'): 'TqROS3_in',
    ('dropTailQueue_ROS4', 'packetPopped'): 'TqROS4_out',
    ('packetServer1', 'packetServed'): 'Tp2_out',
    ('activePacketSource', 'packetCreated'): 'T0_rosbag',
    ('packetServerROSReplayer', 'packetServed'): 'T1_replayer',
    # LabView
    ('dropTailQueueLV0', 'packetPushed'): 'TqLV0_in',
    ('dropTailQueueLV0', 'packetPopped'): 'TqLV0_out',
    ('dropTailQueueLV1', 'packetPushed'): 'TqLV1_in',
    ('dropTailQueueLV1', 'packetPopped'): 'TqLV1_out',
    ('dropTailQueueLV2', 'packetPushed'): 'TqLV2_in',
    ('dropTailQueueLV2', 'packetPopped'): 'TqLV2_out',
    ('Sink', 'packetDropped'): 'Tp4_out'
}

readin_len = {
    # ros
    ('dropTailQueue_ROS1', 'queueLength'): 'QROS1_len',
    ('dropTailQueue_ROS1', 'queueingTime'): 'QROS1_qtime',
    ('dropTailQueue_ROS2', 'queueLength'): 'QROS2_len',
    ('dropTailQueue_ROS2', 'queueingTime'): 'QROS2_qtime',
    ('dropTailQueue_ROS3', 'queueLength'): 'QROS3_len',
    ('dropTailQueue_ROS3', 'queueingTime'): 'QROS3_qtime',
    ('dropTailQueue_ROS4', 'queueLength'): 'QROS4_len',
    ('dropTailQueue_ROS4', 'queueingTime'): 'QROS4_qtime',
    # LabView
    ('dropTailQueueLV0', 'queueLength'): 'QLV0_len',
    ('dropTailQueueLV0', 'queueingTime'): 'QLV0_qtime',
    ('dropTailQueueLV1', 'queueLength'): 'QLV1_len',
    ('dropTailQueueLV1', 'queueingTime'): 'QLV1_qtime',
    ('dropTailQueueLV2', 'queueLength'): 'QLV2_len',
    ('dropTailQueueLV2', 'queueingTime'): 'QLV2_qtime'
}

# mapping
mapping = {
    'T0_rosbag': ('T0', 'dT0', 'B0'),
    'T1_replayer': ('T1', 'dT1', 'B1'),
    'TqROS2_out': ('T2', 'dT2', 'B2'),
    'TqROS3_in': ('T3', 'dT3', 'B3'),
    'TqROS4_out': ('T4', 'dT4', 'B4'),
    'Tp2_out': ('T5', 'dT5', 'B5'),
    'TqLV0_out': ('T6', 'dT6', 'B6'), # vllt mit key TqLV1_in ??
    'TqLV1_out': ('T7', 'dT7', 'B7'),
    'TqLV2_in': ('T8', 'dT8', 'B8'),
    'TqLV2_out': ('T9', 'dT9', 'B9'),
    'Tp4_out': ('T10', 'dT10', 'B10'),
}
