/***************************************************************************
    qgsheatmaprenderer.h
    ---------------------
    begin                : November 2014
    copyright            : (C) 2014 Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSHEATMAPRENDERER_H
#define QGSHEATMAPRENDERER_H

#include "qgis.h"
#include "qgsrenderer.h"
#include "qgssymbol.h"
#include "qgsexpression.h"
#include "qgsgeometry.h"
#include <QScopedPointer>

class QgsColorRamp;

/** \ingroup core
 * \class QgsHeatmapRenderer
 * \brief A renderer which draws points as a live heatmap
 * \note Added in version 2.7
 */
class CORE_EXPORT QgsHeatmapRenderer : public QgsFeatureRenderer
{
  public:

    QgsHeatmapRenderer();
    virtual ~QgsHeatmapRenderer();

    //reimplemented methods
    virtual QgsHeatmapRenderer* clone() const override;
    virtual void startRender( QgsRenderContext& context, const QgsFields& fields ) override;
    virtual bool renderFeature( QgsFeature& feature, QgsRenderContext& context, int layer = -1, bool selected = false, bool drawVertexMarker = false ) override;
    virtual void stopRender( QgsRenderContext& context ) override;
    //! @note symbolForFeature2 in python bindings
    virtual QgsSymbol* symbolForFeature( QgsFeature& feature, QgsRenderContext &context ) override;
    //! @note symbol2 in python bindings
    virtual QgsSymbolList symbols( QgsRenderContext &context ) override;
    virtual QString dump() const override;
    virtual QSet<QString> usedAttributes() const override;
    static QgsFeatureRenderer* create( QDomElement& element );
    virtual QDomElement save( QDomDocument& doc ) override;
    static QgsHeatmapRenderer* convertFromRenderer( const QgsFeatureRenderer* renderer );

    //reimplemented to extent the request so that points up to heatmap's radius distance outside
    //visible area are included
    virtual void modifyRequestExtent( QgsRectangle& extent, QgsRenderContext& context ) override;

    //heatmap specific methods

    /** Returns the color ramp used for shading the heatmap.
     * @returns color ramp for heatmap
     * @see setColorRamp
     */
    QgsColorRamp* colorRamp() const { return mGradientRamp; }

    /** Sets the color ramp to use for shading the heatmap.
     * @param ramp color ramp for heatmap. Ownership of ramp is transferred to the renderer.
     * @see colorRamp
     */
    void setColorRamp( QgsColorRamp* ramp );

    /** Returns the radius for the heatmap
     * @returns heatmap radius
     * @see setRadius
     * @see radiusUnit
     * @see radiusMapUnitScale
     */
    double radius() const { return mRadius; }

    /** Sets the radius for the heatmap
     * @param radius heatmap radius
     * @see radius
     * @see setRadiusUnit
     * @see setRadiusMapUnitScale
     */
    void setRadius( const double radius ) { mRadius = radius; }

    /** Returns the units used for the heatmap's radius
     * @returns units for heatmap radius
     * @see radius
     * @see setRadiusUnit
     * @see radiusMapUnitScale
     */
    QgsUnitTypes::RenderUnit radiusUnit() const { return mRadiusUnit; }

    /** Sets the units used for the heatmap's radius
     * @param unit units for heatmap radius
     * @see radiusUnit
     * @see setRadius
     * @see radiusMapUnitScale
     */
    void setRadiusUnit( const QgsUnitTypes::RenderUnit unit ) { mRadiusUnit = unit; }

    /** Returns the map unit scale used for the heatmap's radius
     * @returns map unit scale for heatmap's radius
     * @see radius
     * @see radiusUnit
     * @see setRadiusMapUnitScale
     */
    const QgsMapUnitScale& radiusMapUnitScale() const { return mRadiusMapUnitScale; }

    /** Sets the map unit scale used for the heatmap's radius
     * @param scale map unit scale for heatmap's radius
     * @see setRadius
     * @see setRadiusUnit
     * @see radiusMapUnitScale
     */
    void setRadiusMapUnitScale( const QgsMapUnitScale& scale ) { mRadiusMapUnitScale = scale; }

    /** Returns the maximum value used for shading the heatmap.
     * @returns maximum value for heatmap shading. If 0, then maximum value will be automatically
     * calculated.
     * @see setMaximumValue
     */
    double maximumValue() const { return mExplicitMax; }

    /** Sets the maximum value used for shading the heatmap.
     * @param value maximum value for heatmap shading. Set to 0 for automatic calculation of
     * maximum value.
     * @see maximumValue
     */
    void setMaximumValue( const double value ) { mExplicitMax = value; }

    /** Returns the render quality used for drawing the heatmap.
     * @returns render quality. A value of 1 indicates maximum quality, and increasing the
     * value will result in faster drawing but lower quality rendering.
     * @see setRenderQuality
     */
    double renderQuality() const { return mRenderQuality; }

    /** Sets the render quality used for drawing the heatmap.
     * @param quality render quality. A value of 1 indicates maximum quality, and increasing the
     * value will result in faster drawing but lower quality rendering.
     * @see renderQuality
     */
    void setRenderQuality( const int quality ) { mRenderQuality = quality; }

    /** Returns the expression used for weighting points when generating the heatmap.
     * @returns point weight expression. If empty, all points are equally weighted.
     * @see setWeightExpression
     */
    QString weightExpression() const { return mWeightExpressionString; }

    /** Sets the expression used for weighting points when generating the heatmap.
     * @param expression point weight expression. If set to empty, all points are equally weighted.
     * @see weightExpression
     */
    void setWeightExpression( const QString& expression ) { mWeightExpressionString = expression; }

  private:
    //! Private copy constructor. @see clone()
    QgsHeatmapRenderer( const QgsHeatmapRenderer& );
    //! Private assignment operator. @see clone()
    QgsHeatmapRenderer& operator=( const QgsHeatmapRenderer& );

    QVector<double> mValues;

    double mCalculatedMaxValue;

    double mRadius;
    int mRadiusPixels;
    double mRadiusSquared;
    QgsUnitTypes::RenderUnit mRadiusUnit;
    QgsMapUnitScale mRadiusMapUnitScale;

    QString mWeightExpressionString;
    int mWeightAttrNum;
    QScopedPointer<QgsExpression> mWeightExpression;

    QgsColorRamp* mGradientRamp;

    double mExplicitMax;
    int mRenderQuality;

    int mFeaturesRendered;

    double uniformKernel( const double distance, const int bandwidth ) const;
    double quarticKernel( const double distance, const int bandwidth ) const;
    double triweightKernel( const double distance, const int bandwidth ) const;
    double epanechnikovKernel( const double distance, const int bandwidth ) const;
    double triangularKernel( const double distance, const int bandwidth ) const;

    QgsMultiPoint convertToMultipoint( const QgsGeometry *geom );
    void initializeValues( QgsRenderContext& context );
    void renderImage( QgsRenderContext &context );
};


#endif // QGSHEATMAPRENDERER_H
