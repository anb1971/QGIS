/***************************************************************************
                              qgswmsprojectparser.h
                              ---------------------
  begin                : March 25, 2014
  copyright            : (C) 2014 by Marco Hugentobler
  email                : marco dot hugentobler at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSWMSPROJECTPARSER_H
#define QGSWMSPROJECTPARSER_H

#include "qgswmsconfigparser.h"
#include "qgsserverprojectparser.h"
#include "qgslayertreegroup.h"

#ifdef HAVE_SERVER_PYTHON_PLUGINS
class QgsAccessControl;
#endif

class QTextDocument;
class QSvgRenderer;
class QgsMapSettings;

class SERVER_EXPORT QgsWmsProjectParser : public QgsWmsConfigParser
{
  public:
    QgsWmsProjectParser(
      const QString& filePath
#ifdef HAVE_SERVER_PYTHON_PLUGINS
      , const QgsAccessControl* accessControl
#endif
    );
    virtual ~QgsWmsProjectParser();

    /** Adds layer and style specific capabilities elements to the parent node. This includes the individual layers and styles, their description, native CRS, bounding boxes, etc.
        @param fullProjectInformation If true: add extended project information (does not validate against WMS schema)*/
    void layersAndStylesCapabilities( QDomElement& parentElement, QDomDocument& doc, const QString& version, bool fullProjectSettings = false ) const override;

    QList<QgsMapLayer*> mapLayerFromStyle( const QString& lName, const QString& styleName, bool useCache = true ) const override;

    QString serviceUrl() const override;

    QStringList wfsLayerNames() const override;

    void owsGeneralAndResourceList( QDomElement& parentElement, QDomDocument& doc, const QString& strHref ) const override;

    //legend
    double legendBoxSpace() const override;
    double legendLayerSpace() const override;
    double legendLayerTitleSpace() const override;
    double legendSymbolSpace() const override;
    double legendIconLabelSpace() const override;
    double legendSymbolWidth() const override;
    double legendSymbolHeight() const override;
    const QFont& legendLayerFont() const override;
    const QFont& legendItemFont() const override;

    double maxWidth() const override;
    double maxHeight() const override;
    double imageQuality() const override;
    int wmsPrecision() const override;

    // WMS inspire capabilities
    bool wmsInspireActivated() const override;
    void inspireCapabilities( QDomElement& parentElement, QDomDocument& doc ) const override;

    //printing
    QgsComposition* initComposition( const QString& composerTemplate, const QgsMapSettings& mapSettings, QList< QgsComposerMap* >& mapList, QList< QgsComposerLegend* >& legendList, QList< QgsComposerLabel* >& labelList, QList<const QgsComposerHtml *>& htmlFrameList ) const override;
    void printCapabilities( QDomElement& parentElement, QDomDocument& doc ) const override;

    //todo: fixme
    void setScaleDenominator( double ) override {}
    void addExternalGMLData( const QString&, QDomDocument* ) override {}

    QList< QPair< QString, QgsDatumTransformStore::Entry > > layerCoordinateTransforms() const override;

    //! Fills a layer and a style list. The two list have the same number of entries and the style and the layer at a position belong together (similar to the HTTP parameters 'Layers' and 'Styles'. Returns 0 in case of success
    int layersAndStyles( QStringList& layers, QStringList& styles ) const override;

    //! Returns the xml fragment of a style
    QDomDocument getStyle( const QString& styleName, const QString& layerName ) const override;

    //! Returns the xml fragment of layers styles
    QDomDocument getStyles( QStringList& layerList ) const override;

    //! Returns the xml fragment of layers styles description
    QDomDocument describeLayer( QStringList& layerList, const QString& hrefString ) const override;

    //! Returns if output are MM or PIXEL
    QgsUnitTypes::RenderUnit outputUnits() const override;

    //! True if the feature info response should contain the wkt geometry for vector features
    bool featureInfoWithWktGeometry() const override;

    //! True if the feature info wkt geometry is delivered with segmentized curve types
    bool segmentizeFeatureInfoWktGeometry() const override;

    //! Returns map with layer aliases for GetFeatureInfo (or 0 pointer if not supported). Key: layer name, Value: layer alias
    QHash<QString, QString> featureInfoLayerAliasMap() const override;

    QString featureInfoDocumentElement( const QString& defaultValue ) const override;

    QString featureInfoDocumentElementNS() const override;

    QString featureInfoSchema() const override;

    //! Return feature info in format SIA2045?
    bool featureInfoFormatSIA2045() const override;

    //! Draw text annotation items from the QGIS projectfile
    void drawOverlays( QPainter* p, int dpi, int width, int height ) const override;

    //! Load PAL engine settings  into global project instance
    void loadLabelSettings() const override;

    int nLayers() const override;

    void serviceCapabilities( QDomElement& parentElement, QDomDocument& doc ) const override;

    bool useLayerIds() const override { return mProjectParser->useLayerIds(); }

    bool allowRequestDefinedDatasources() const override;

  private:
    QgsServerProjectParser* mProjectParser;
#ifdef HAVE_SERVER_PYTHON_PLUGINS
    const QgsAccessControl* mAccessControl;
#endif

    mutable QFont mLegendLayerFont;
    mutable QFont mLegendItemFont;

    //! Watermark text items
    QList< QPair< QTextDocument*, QDomElement > > mTextAnnotationItems;
    //! Watermark items (content cached in QgsSVGCache)
    QList< QPair< QSvgRenderer*, QDomElement > > mSvgAnnotationElems;

    //! Returns an ID-list of layers which are not queryable (comes from <properties> -> <Identify> -> <disabledLayers in the project file
    virtual QStringList identifyDisabledLayers() const override;

    //! Reads layer drawing order from the legend section of the project file and appends it to the parent elemen (usually the <Capability> element)
    void addDrawingOrder( QDomElement& parentElem, QDomDocument& doc, const QHash<QString, QString> &idNameMap, const QStringList &layerIDList ) const;

    void addLayerStyles( QgsMapLayer* currentLayer, QDomDocument& doc, QDomElement& layerElem, const QString& version ) const;

    void addLayers( QDomDocument &doc,
                    QDomElement &parentLayer,
                    const QDomElement &legendElem,
                    QgsLayerTreeGroup *layerTreeGroup,
                    const QMap<QString, QgsMapLayer *> &layerMap,
                    const QStringList &nonIdentifiableLayers,
                    const QString &version, //1.1.1 or 1.3.0
                    bool fullProjectSettings,
                    QHash<QString, QString> &idNameMap,
                    QStringList &layerIDList ) const;

    void addOWSLayerStyles( QgsMapLayer* currentLayer, QDomDocument& doc, QDomElement& layerElem ) const;

    void addOWSLayers( QDomDocument &doc, QDomElement &parentElem, const QDomElement &legendElem,
                       const QMap<QString, QgsMapLayer *> &layerMap, const QStringList &nonIdentifiableLayers,
                       const QString& strHref, QgsRectangle& combinedBBox, const QString& strGroup ) const;

    //! Adds layers from a legend group to list (could be embedded or a normal group)
    void addLayersFromGroup( const QDomElement& legendGroupElem, QMap< int, QgsMapLayer*>& layers, bool useCache = true ) const;

    QDomElement composerByName( const QString& composerName ) const;
    QgsLayerTreeGroup* projectLayerTreeGroup() const;

    static bool annotationPosition( const QDomElement& elem, double scaleFactor, double& xPos, double& yPos );
    static void drawAnnotationRectangle( QPainter* p, const QDomElement& elem, double scaleFactor, double xPos, double yPos, int itemWidth, int itemHeight );
    void createTextAnnotationItems();
    void createSvgAnnotationItems();
    void cleanupSvgAnnotationItems();
    void cleanupTextAnnotationItems();

    QString getCapaServiceUrl( QDomDocument& doc ) const;

    void readLabelSettings( int& searchMethod, int& nCandPoint, int& nCandLine, int& nCandPoly, bool& showingCandidates, bool& drawRectOnly, bool& showingShadowRects, bool& showingAllLabels, bool& showingPartialsLabels, bool& drawOutlineLabels ) const;
};

#endif // QGSWMSPROJECTPARSER_H
