/***************************************************************************
    qgswebviewwidgetconfigdlgbase.cpp
     --------------------------------------
    Date                 : 11.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgswebviewconfigdlg.h"

QgsWebViewWidgetConfigDlg::QgsWebViewWidgetConfigDlg( QgsVectorLayer* vl, int fieldIdx, QWidget *parent )
    :  QgsEditorConfigWidget( vl, fieldIdx, parent )
{
  setupUi( this );
  connect( sbWidgetHeight, SIGNAL( valueChanged( int ) ), this, SIGNAL( changed() ) );
  connect( sbWidgetWidth, SIGNAL( valueChanged( int ) ), this, SIGNAL( changed() ) );
}

QVariantMap QgsWebViewWidgetConfigDlg::config()
{
  QVariantMap cfg;

  cfg.insert( QStringLiteral( "Height" ), sbWidgetHeight->value() );
  cfg.insert( QStringLiteral( "Width" ), sbWidgetWidth->value() );

  return cfg;
}

void QgsWebViewWidgetConfigDlg::setConfig( const QVariantMap& config )
{
  sbWidgetHeight->setValue( config.value( QStringLiteral( "Height" ), 0 ).toInt() );
  sbWidgetWidth->setValue( config.value( QStringLiteral( "Width" ), 0 ).toInt() );
}
