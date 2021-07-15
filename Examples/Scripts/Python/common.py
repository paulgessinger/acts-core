def getOpenDataDetector():
    import acts.examples.dd4hep

    dd4hepConfig = acts.examples.dd4hep.DD4hepGeometryService.Config(
        xmlFileNames=["thirdparty/OpenDataDetector/xml/OpenDataDetector.xml"]
    )
    detector = acts.examples.dd4hep.DD4hepDetector()

    config = acts.MaterialMapJsonConverter.Config()
    matDeco = acts.JsonMaterialDecorator(
        rConfig=config,
        jFileName="thirdparty/OpenDataDetector/config/odd-material-mapping.config",
        level=acts.logging.ERROR,
    )

    trackingGeometry, deco = detector.finalize(dd4hepConfig, matDeco)

    return detector, trackingGeometry, deco
