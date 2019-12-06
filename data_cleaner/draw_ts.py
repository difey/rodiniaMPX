import pandas as pd
import os
import matplotlib.pyplot as plt
import numpy as np

result_root_dir = "/chpc/users/JKChen/PAPI_test_result"
arch_name = "kp920"
app_name = "neutral"

def prepare_plot_data(data, start, interval):
    # data_list = np.log10(data).tolist()
    data_list = data.tolist()
    data_out = [x for x in data_list]
    # for i in range(1, len(data_list)):
    #     data_out[i] = data_list[i] - data_list[i-1]

    # data_out = data_out[start:start+interval]

    # return data_out, [x for x in range(start, start+interval)]
    return data_out, [x for x in range(len(data_out))]

def plot_data_diff(ocoe_data, mpx_data, color, label):
    length = min(np.size(ocoe_data), np.size(mpx_data))
    diff = mpx_data[0:length] - ocoe_data[0:length]
    x_axis = [x+1 for x in range(np.size(diff))]
    
    
    plt.plot(x_axis, diff.tolist(), color=color, label=label)

def draw_event_ts(result_dir, arch_name, app_name, id, eventname):
    resultfile_dir = os.path.join(result_dir, arch_name, app_name)

    #ocoe ts
    fileid = int(id / 10) + 1
    ocoe_filename = os.path.join(resultfile_dir, "result_{}_ocoe_{}.csv".format(app_name, fileid))
    ocoe_df = pd.read_csv(ocoe_filename)
    ocoe_data = ocoe_df[eventname].values
    # ocoe_data = np.log10(ocoe_data + 1)

    #mpx 20
    fileid = int(id / 20) + 1
    mpx20_filename = os.path.join(resultfile_dir, "result_{}_mpx_20_{}.csv".format(app_name, fileid))
    mpx20_df = pd.read_csv(mpx20_filename)
    mpx20_data = mpx20_df[eventname].values
    # mpx20_data = np.log10(mpx20_data + 1)

    #mpx 40
    fileid = int(id / 40) + 1
    mpx40_filename = os.path.join(resultfile_dir, "result_{}_mpx_40_{}.csv".format(app_name, fileid))
    mpx40_df = pd.read_csv(mpx40_filename)
    mpx40_data = mpx40_df[eventname].values
    # mpx40_data = np.log10(mpx40_data + 1)

    #mpx 80
    fileid = int(id / 80) + 1
    mpx80_filename = os.path.join(resultfile_dir, "result_{}_mpx_80_{}.csv".format(app_name, fileid))
    mpx80_df = pd.read_csv(mpx80_filename)
    mpx80_data = mpx80_df[eventname].values
    # mpx80_data = np.log10(mpx80_data + 1)

    #mpx all
    mpxall_filename = os.path.join(resultfile_dir, "result_{}_mpx_155.csv".format(app_name, fileid))
    mpxall_df = pd.read_csv(mpxall_filename)
    mpxall_data = mpxall_df[eventname].values
    # mpxall_data = np.log10(mpxall_data + 1)

    # ocoe_x_axis = [x+1 for x in range(np.size(ocoe_data))]
    # mpx20_x_axis = [x+1 for x in range(np.size(mpx20_data))]
    # mpx40_x_axis = [x+1 for x in range(np.size(mpx40_data))]
    # mpx80_x_axis = [x+1 for x in range(np.size(mpx80_data))]
    # mpxall_x_axis = [x+1 for x in range(np.size(mpxall_data))]

    ocoe_data_list, ocoe_x_axis = prepare_plot_data(ocoe_data, 250000, 1000)
    mpx20_data_list, mpx20_x_axis = prepare_plot_data(mpx20_data, 250000, 1000)
    mpx40_data_list, mpx40_x_axis = prepare_plot_data(mpx40_data, 250000, 1000)
    mpx80_data_list, mpx80_x_axis = prepare_plot_data(mpx80_data, 250000, 1000)
    mpxall_data_list, mpxall_x_axis = prepare_plot_data(mpxall_data, 250000, 1000)

    plt.title(eventname)
    plt.plot(ocoe_x_axis, ocoe_data_list, color="red", label="OCOE")
    plt.plot(mpx20_x_axis, mpx20_data_list, color="skyblue", label="MLPX_20")
    plt.plot(mpx40_x_axis, mpx40_data_list, color="blue", label="MLPX_40")
    plt.plot(mpx80_x_axis, mpx80_data_list, color="green", label="MLPX_80")
    plt.plot(mpxall_x_axis, mpxall_data_list, color="blueviolet", label="MLPX_155")
    plt.legend(loc="upper right")
    plt.xlabel("Sampling time(x 10^6 cyc)")
    plt.ylabel("Event count")

    outputfilename = os.path.join(resultfile_dir, "total", "{}.png".format(eventname))
    plt.savefig(outputfilename)
    plt.cla()

    # ymin = min(min(ocoe_data_list), min(mpx20_data_list), min(mpx40_data_list), min(mpx80_data_list), min(mpxall_data_list))
    # ymax = max(max(ocoe_data_list), max(mpx20_data_list), max(mpx40_data_list), max(mpx80_data_list), max(mpxall_data_list))

    # plt.title("{}_OCOE".format(eventname))
    # plt.ylim(ymin-10,ymax+10)
    # plt.plot(ocoe_x_axis, ocoe_data_list, color="red", label="OCOE")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event count change")

    # outputfilename = os.path.join(resultfile_dir, "change", "{}_OCOE.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla()

    # plt.title("{}_MLPX20".format(eventname))
    # plt.ylim(ymin-10,ymax+10)
    # plt.plot(mpx20_x_axis, mpx20_data_list, color="skyblue", label="MLPX_20")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event count change")

    # outputfilename = os.path.join(resultfile_dir, "change", "{}_MLPX20.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla()

    # plt.title("{}_MLPX40".format(eventname))
    # plt.ylim(ymin-10,ymax+10)
    # plt.plot(mpx40_x_axis, mpx40_data_list, color="blue", label="MLPX_40")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event count change")

    # outputfilename = os.path.join(resultfile_dir, "change", "{}_MLPX40.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla()

    # plt.title("{}_MLPX80".format(eventname))
    # plt.ylim(ymin-10,ymax+10)
    # plt.plot(mpx80_x_axis, mpx80_data_list, color="green", label="MLPX_80")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event count change")

    # outputfilename = os.path.join(resultfile_dir, "change", "{}_MLPX80.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla()

    # plt.title("{}_MLPX155".format(eventname))
    # plt.ylim(ymin-10,ymax+10)
    # plt.plot(mpxall_x_axis, mpxall_data_list, color="blueviolet", label="MLPX_155")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event count change")

    # outputfilename = os.path.join(resultfile_dir, "change", "{}_MPX155.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla() 

    # plt.title(eventname)
    # plot_data_diff(ocoe_data, mpx20_data, "skyblue", "MLPX_20")
    # plot_data_diff(ocoe_data, mpx40_data, "blue", "MLPX_40")
    # plot_data_diff(ocoe_data, mpx80_data, "green", "MLPX_80")
    # plot_data_diff(ocoe_data, mpxall_data, "purple", "MLPX_155")
    # plt.xlabel("Sampling time(x 10^6 cyc)")
    # plt.ylabel("Event num(MLPX-OCOE)")
    # plt.legend(loc="upper right")

    # outputfilename = os.path.join(resultfile_dir, "diff", "{}.png".format(eventname))
    # plt.savefig(outputfilename)
    # plt.cla()

def draw_ts(result_dir, arch_name, app_name):
    resultfile_dir = os.path.join(result_dir, arch_name, app_name)

    if arch_name == "kp920":
        mpx_filename = os.path.join(resultfile_dir, "result_{}_mpx_155.csv".format(app_name))

    mpx_df = pd.read_csv(mpx_filename)
    event_list = mpx_df.columns.values.tolist()
    # print(event_list)

    for id in range(len(event_list)):
        draw_event_ts(result_dir, arch_name, app_name, id, event_list[id])

if __name__ == "__main__":
    draw_ts(result_root_dir, arch_name, app_name)
    # draw_event_ts(result_root_dir, arch_name, app_name, 1, "L1I_CACHE_REFILL")