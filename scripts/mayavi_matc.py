
import mayavi.mlab as mlab
import xml_loader as loader


CHAIN = loader.xml_dataloader("../build/chain_nor.xml","Local_Nor")


mlab.figure(size=(1920,1080))

barc = mlab.barchart(CHAIN,lateral_scale=1, colormap='viridis', scale_factor=0.5)#colormap='viridis'


mlab.colorbar(object= barc,orientation='vertical',nb_labels=10)
mlab.view(azimuth=270,elevation=0)
mlab.gcf().scene.parallel_projection = True


cam = barc.scene.camera

cam.zoom(1.2)
mlab.draw()

# mlab.savefig(filename='color_'+file_name)

mlab.show()

