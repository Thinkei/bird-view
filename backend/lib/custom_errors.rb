# frozen_string_literal: true

# To add a new error, please add new message in en.yml
# path: messages.errors with snake case version of error class name

module CustomErrors
  class BaseError < StandardError
    def initialize(message = nil)
      super message || I18n.t("messages.errors.#{code}")
    end

    def code
      self.class.name.demodulize.underscore
    end
  end

  class Unauthenticated < BaseError; end
  class InvalidArgument < BaseError; end
end
