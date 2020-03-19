# frozen_string_literal: true

class HealthzController < ActionController::API
  def index
    if ActiveRecord::Base.connection.active?
      render status: 200
    else
      render status: 503
    end
  end
end
